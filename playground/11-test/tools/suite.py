"""11단계 일괄 테스트. run.sh 가 부른다 (직접: python3 tools/suite.py [--no-leaks] [--no-time])

단계
  1) 원본 씬 전부: 기준 판정(ref_rules) vs 팀 파서 판정, "Error\\n" 출력, ASAN(격자 렌더)
  2) 원본 씬 전부: leaks -atExit (파서 오류 경로의 누수)
  3) 렌더 대상 = 파서가 받은 원본 + (거부된 원본의) 변환본
       probe_o2   → BMP, NaN/범위, 도형에 맞은 픽셀 수
       probe_team → 루트 Makefile 플래그로 전체 해상도 1장 시간 (순차 실행)
  4) 결과: out/results.tsv, out/summary.txt, out/sheets/*.png (썸네일 모음)
"""
import concurrent.futures as cf
import glob
import os
import re
import struct
import subprocess
import sys
import time
import zlib

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)
from ref_rules import classify, convert  # noqa: E402

os.chdir(ROOT)
B = '.build'
OUT = 'out'
JOBS = os.cpu_count() or 4


def sh(cmd, timeout):
    t0 = time.time()
    try:
        p = subprocess.run(cmd, capture_output=True, timeout=timeout)
        return p.returncode, p.stdout.decode('utf-8', 'replace'), \
            p.stderr.decode('utf-8', 'replace'), time.time() - t0
    except subprocess.TimeoutExpired:
        return 'TIMEOUT', '', '', time.time() - t0


def probe_fields(stdout):
    m = re.search(r'PROBE (.*)', stdout)
    if not m:
        return {}
    return dict(kv.split('=') for kv in m.group(1).split())


def scene_list():
    files = []
    for group, pat in (('check', 'scenes/check/*.rt'),
                       ('repo-valid', '../../scenes/valid/*.rt'),
                       ('repo-invalid', '../../scenes/invalid/*.rt'),
                       ('edge', 'scenes/edge/*.rt'),
                       ('web', 'scenes/web/*/*.rt')):
        for f in sorted(glob.glob(pat)):
            rel = f.replace('../../', 'repo/').replace('scenes/web/', 'web/')
            rel = rel.replace('scenes/check/', 'check/').replace('scenes/edge/', 'edge/')
            files.append({'group': group, 'file': f,
                          'id': rel.replace('/', '__')[:-3]})
    return files


# ---------------------------------------------------------------------------
def stage_parse(rows):
    def one(r):
        data = open(r['file'], 'rb').read()
        r['ref'], errs, pol = classify(data)
        r['why'] = errs[0] if errs else ''
        r['policy'] = ','.join(sorted(pol))
        code, out, err, _ = sh([f'{B}/probe_asan', '-g', '8', r['file']], 300)
        f = probe_fields(out)
        r['parse'] = f.get('parse', '?')
        r['asan'] = 'FAIL' if 'AddressSanitizer' in err else (
            'TIMEOUT' if code == 'TIMEOUT' else 'ok')
        r['asan_code'] = str(code)
        if r['parse'] == '0':
            r['error_msg'] = 'ok' if err.startswith('Error\n') else 'NO-Error-PREFIX'
            r['error_text'] = err.split('\n')[1] if err.startswith('Error\n') else err[:60]
        else:
            r['error_msg'] = '-'
            r['error_text'] = ''
        if r['ref'] == 'VALID':
            r['verdict'] = 'ok' if r['parse'] == '1' else 'MISMATCH(거부됨)'
        elif r['ref'] == 'INVALID':
            r['verdict'] = 'ok' if r['parse'] == '0' else 'MISMATCH(받아줌)'
        else:
            r['verdict'] = 'policy:' + ('받음' if r['parse'] == '1' else '거부')
        if r['asan'] != 'ok':
            r['verdict'] += ' ASAN'
        return r
    with cf.ThreadPoolExecutor(JOBS) as ex:
        list(ex.map(one, rows))


def stage_leaks(rows):
    def one(r):
        code, out, err, _ = sh(['leaks', '-atExit', '--', f'{B}/probe_team', '-p',
                                r['file']], 120)
        m = re.search(r'(\d+) leaks? for (\d+) total leaked bytes', out + err)
        r['leaks'] = m.group(1) if m else f'?({code})'
        return r
    with cf.ThreadPoolExecutor(JOBS) as ex:
        list(ex.map(one, rows))


def stage_render(rows, do_time):
    os.makedirs(f'{OUT}/converted', exist_ok=True)
    os.makedirs(f'{OUT}/bmp', exist_ok=True)
    targets = []
    for r in rows:
        r['render'] = ''
        if r['parse'] == '1':
            r['render_src'] = r['file']
            r['render'] = 'orig'
        elif r['group'] == 'web':
            data, dropped = convert(open(r['file'], 'rb').read())
            v, _, _ = classify(data)
            if v != 'INVALID':
                path = f"{OUT}/converted/{r['id']}.rt"
                open(path, 'wb').write(data)
                r['render_src'] = path
                r['render'] = 'conv'
                r['dropped'] = ' '.join(dropped)
        if r['render']:
            targets.append(r)

    def one(r):
        bmp = f"{OUT}/bmp/{r['id']}.bmp"
        code, out, err, _ = sh([f'{B}/probe_o2', '-o', bmp, r['render_src']], 600)
        f = probe_fields(out)
        r['o2_parse'] = f.get('parse', '?')
        r['bad'] = f.get('bad', '?')
        r['hit'] = f.get('hit', '?')
        r['objs'] = f"sp{f.get('sp', '?')} pl{f.get('pl', '?')} cy{f.get('cy', '?')}"
        r['ms_o2'] = f.get('ms', '?')
        if r['render'] == 'conv':
            c2, o2, e2, _ = sh([f'{B}/probe_asan', '-g', '8', r['render_src']], 600)
            r['asan_conv'] = 'FAIL' if 'AddressSanitizer' in e2 else 'ok'
        return r
    with cf.ThreadPoolExecutor(JOBS) as ex:
        list(ex.map(one, targets))
    if do_time:
        for r in sorted(targets, key=lambda r: r['id']):
            code, out, err, _ = sh([f'{B}/probe_team', r['render_src']], 600)
            r['ms_team'] = probe_fields(out).get('ms', str(code))
    return targets


# ---------------------------------------------------------------------------
def read_bmp(path):
    d = open(path, 'rb').read()
    w, h = struct.unpack('<ii', d[18:26])
    return w, h, (w * 3 + 3) & ~3, d


def write_png(path, w, h, rgb):
    raw = b''.join(b'\x00' + bytes(rgb[y * w * 3:(y + 1) * w * 3]) for y in range(h))

    def chunk(t, dat):
        return struct.pack('>I', len(dat)) + t + dat + \
            struct.pack('>I', zlib.crc32(t + dat) & 0xffffffff)
    open(path, 'wb').write(b'\x89PNG\r\n\x1a\n'
                           + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0))
                           + chunk(b'IDAT', zlib.compress(raw, 6)) + chunk(b'IEND', b''))


def sheets(targets, cols=5, rows_per=4, tw=240, th=180):
    os.makedirs(f'{OUT}/sheets', exist_ok=True)
    per = cols * rows_per
    index = []
    for s in range(0, len(targets), per):
        page = targets[s:s + per]
        W, H = cols * tw, rows_per * th
        canvas = bytearray(b'\x28' * (W * H * 3))
        for i, r in enumerate(page):
            w, h, row, d = read_bmp(f"{OUT}/bmp/{r['id']}.bmp")
            ox, oy = (i % cols) * tw, (i // cols) * th
            for y in range(th - 2):
                sy = h - 1 - (y * h // th)
                o = 54 + row * sy
                for x in range(tw - 2):
                    sx = x * w // tw
                    b, g, rr = d[o + sx * 3:o + sx * 3 + 3]
                    k = ((oy + y) * W + ox + x) * 3
                    canvas[k:k + 3] = bytes((rr, g, b))
            index.append(f'sheet_{s // per + 1:02d}  #{i + 1:2d}  {r["id"]}')
        write_png(f'{OUT}/sheets/sheet_{s // per + 1:02d}.png', W, H, canvas)
    open(f'{OUT}/sheets/index.txt', 'w').write('\n'.join(index) + '\n')


# ---------------------------------------------------------------------------
COLS = ['group', 'id', 'ref', 'policy', 'why', 'parse', 'verdict', 'error_msg',
        'error_text', 'asan', 'leaks', 'render', 'dropped', 'objs', 'bad', 'hit',
        'ms_o2', 'ms_team', 'asan_conv']


def main():
    do_leaks = '--no-leaks' not in sys.argv
    do_time = '--no-time' not in sys.argv
    os.makedirs(OUT, exist_ok=True)
    rows = scene_list()
    t0 = time.time()
    print(f'씬 {len(rows)}개. 1) 파서 판정 + ASAN ...', flush=True)
    stage_parse(rows)
    if do_leaks:
        print(f'  ({time.time() - t0:.0f}s) 2) leaks ...', flush=True)
        stage_leaks(rows)
    print(f'  ({time.time() - t0:.0f}s) 3) 렌더 ...', flush=True)
    targets = stage_render(rows, do_time)
    print(f'  ({time.time() - t0:.0f}s) 4) 썸네일 ...', flush=True)
    sheets(targets)
    with open(f'{OUT}/results.tsv', 'w') as f:
        f.write('\t'.join(COLS) + '\n')
        for r in rows:
            f.write('\t'.join(str(r.get(c, '')) for c in COLS) + '\n')
    lines = summarize(rows, targets)
    open(f'{OUT}/summary.txt', 'w').write('\n'.join(lines) + '\n')
    print('\n'.join(lines))
    print(f'({time.time() - t0:.0f}s)')
    bad = [r for r in rows if 'MISMATCH' in r['verdict'] or r['asan'] != 'ok'
           or (r.get('leaks') not in (None, '0')) or r.get('bad') not in (None, '0')
           or r['error_msg'] == 'NO-Error-PREFIX']
    return 1 if bad else 0


def summarize(rows, targets):
    from collections import Counter
    L = []
    c = Counter((r['group'], r['ref'], r['verdict']) for r in rows)
    L.append('== 파서 판정 (기준 검사기 vs 팀 파서) ==')
    for k in sorted(c):
        L.append(f'  {k[0]:<13} {k[1]:<8} {k[2]:<22} {c[k]}')
    L.append('== 안전 ==')
    L.append(f"  ASAN 실패      {sum(r['asan'] != 'ok' for r in rows)} / {len(rows)}")
    if any('leaks' in r for r in rows):
        L.append(f"  누수 있음      {sum(r.get('leaks') != '0' for r in rows)} / {len(rows)}")
    else:
        L.append("  누수 검사      생략 (--no-leaks)")
    L.append(f"  Error 접두 없음 {sum(r['error_msg'] == 'NO-Error-PREFIX' for r in rows)}")
    L.append('== 렌더 ==')
    L.append(f"  대상 {len(targets)} (원본 {sum(r['render'] == 'orig' for r in targets)}"
             f", 변환본 {sum(r['render'] == 'conv' for r in targets)})")
    L.append(f"  NaN/범위 밖 픽셀 있는 씬 {sum(r.get('bad') != '0' for r in targets)}")
    L.append(f"  변환본 ASAN 실패 {sum(r.get('asan_conv') == 'FAIL' for r in targets)}")
    L.append(f"  도형이 한 픽셀도 안 보이는 씬 {sum(r.get('hit') == '0' for r in targets)}: "
             + ', '.join(r['id'] for r in targets if r.get('hit') == '0'))
    ms = sorted((float(r['ms_team']), r['id']) for r in targets
                if re.match(r'^[\d.]+$', str(r.get('ms_team', ''))))
    if ms:
        L.append(f"  전체 해상도 1장 (루트 Makefile 플래그): 중앙값 {ms[len(ms) // 2][0]:.0f} ms,"
                 f" 최대 {ms[-1][0]:.0f} ms ({ms[-1][1]})")
    return L


if __name__ == '__main__':
    sys.exit(main())
