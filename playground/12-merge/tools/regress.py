"""12단계 렌더 회귀 - 파일을 나눈 srcs/render 가 10단계 폴더와 픽셀까지 같은가

  A  srcs/render 전부      + 서연 파서 최신본(origin/main)   = 합친 결과
  B  playground/10-interaction + 서연 파서 최신본           = 옮기기 전 렌더러
  C  11-test/out/bmp  (10-interaction + 머지 전 파서)       = 11단계에서 뽑아 둔 그림

  A == B 면 파일 분리가 계산을 안 바꿨다
  B == C 면 파서 리팩터링이 파싱 결과(숫자)를 안 바꿨다

씬 목록은 11단계 out/results.tsv 에서 렌더한 175개 (원본 + 변환본)
"""
import concurrent.futures as cf
import csv
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
STEP = os.path.dirname(HERE)
T11 = os.path.join(os.path.dirname(STEP), '11-test')
OUT = os.path.join(STEP, '.build', 'regress')


def scene_path(r):
    parts = r['id'].split('__')
    if r['render'] == 'conv':
        return os.path.join(T11, 'out', 'converted', r['id'] + '.rt')
    g = r['group']
    if g == 'check':
        return os.path.join(T11, 'scenes', 'check', parts[1] + '.rt')
    if g == 'edge':
        return os.path.join(T11, 'scenes', 'edge', parts[1] + '.rt')
    if g == 'repo-valid':
        return os.path.join(STEP, '..', '..', 'scenes', 'valid', parts[-1] + '.rt')
    return os.path.join(T11, 'scenes', 'web', parts[1] + '__' + parts[2], '__'.join(parts[3:]) + '.rt')


def run(probe, scene, bmp):
    p = subprocess.run([probe, '-o', bmp, scene], capture_output=True, timeout=900)
    return p.returncode, p.stdout.decode()


def main():
    probe_a, probe_b = sys.argv[1], sys.argv[2]
    rows = [r for r in csv.DictReader(open(os.path.join(T11, 'out', 'results.tsv')), delimiter='\t') if r['render']]
    missing = [r['id'] for r in rows if not os.path.exists(scene_path(r))]
    if missing:
        print('씬 파일을 못 찾음:', missing[:5])
        return 2
    for d in ('A', 'B'):
        os.makedirs(os.path.join(OUT, d), exist_ok=True)

    def one(r):
        s = scene_path(r)
        a = os.path.join(OUT, 'A', r['id'] + '.bmp')
        b = os.path.join(OUT, 'B', r['id'] + '.bmp')
        ca, _ = run(probe_a, s, a)
        cb, _ = run(probe_b, s, b)
        c = os.path.join(T11, 'out', 'bmp', r['id'] + '.bmp')
        same_ab = open(a, 'rb').read() == open(b, 'rb').read() if os.path.exists(a) and os.path.exists(b) else False
        same_bc = open(b, 'rb').read() == open(c, 'rb').read() if os.path.exists(c) else None
        return r['id'], ca, cb, same_ab, same_bc

    with cf.ThreadPoolExecutor(os.cpu_count() or 4) as ex:
        res = list(ex.map(one, rows))
    ab_diff = [x for x in res if not x[3]]
    bc_diff = [x for x in res if x[4] is False]
    bc_none = [x for x in res if x[4] is None]
    print(f'  씬 {len(res)}개')
    print(f'  A(srcs/render) == B(10-interaction)     : {len(res) - len(ab_diff)} / {len(res)}')
    for x in ab_diff[:10]:
        print('    다름:', x[0], 'exit', x[1], x[2])
    print(f'  B(새 파서)     == C(11단계, 머지 전 파서): {len(res) - len(bc_diff) - len(bc_none)} / {len(res)}'
          + (f'  (비교 그림 없음 {len(bc_none)})' if bc_none else ''))
    for x in bc_diff[:10]:
        print('    다름:', x[0])
    return 1 if ab_diff or bc_diff else 0


if __name__ == '__main__':
    sys.exit(main())
