"""11단계 기준 검사기 — 팀 파서와 '따로' 짠 subject 필수 규칙.

두 구현이 같은 파일에 다른 판정을 내리면, 둘 중 하나가 틀렸거나 규칙이 애매한 곳이다.

판정
  VALID    필수 규칙을 전부 지킴           → 파서가 받아야 한다
  INVALID  규칙 위반이 하나라도 있음        → 파서가 "Error\\n" 으로 거부해야 한다
  POLICY   subject 가 딱 잘라 말하지 않는 것 → 받든 거부하든 괜찮다 (어느 쪽인지만 기록)
           comment  '#' 주석 줄 (subject 문법에 없음)
           tab      공백 대신 탭 (subject 는 'one or more space(s)')
           unnorm   범위 [-1,1] 안이지만 길이가 1이 아닌 방향 벡터 (파서는 정규화해서 받음)

subject 필수 규칙 (요약)
  A  ratio[0,1] R,G,B                         한 번만
  C  x,y,z  방향(각 축 [-1,1], 정규화)  fov[0,180]   한 번만
  L  x,y,z  brightness[0,1]  R,G,B             한 번만
  sp x,y,z  지름  R,G,B
  pl x,y,z  법선  R,G,B
  cy x,y,z  축  지름  높이  R,G,B
  색은 [0,255] 정수. 요소 순서 자유, 빈 줄 허용. 잘못된 게 있으면 "Error\\n" + 메시지
"""
import math
import re

NUM = re.compile(r'^[+-]?(\d+(\.\d*)?|\.\d+)$')
INT = re.compile(r'^\d+$')
COUNT = {'A': 3, 'C': 4, 'L': 4, 'sp': 4, 'pl': 4, 'cy': 6}


def _num(tok, errs, what):
    if not NUM.match(tok):
        errs.append(f'{what}: 숫자 아님 "{tok}"')
        return None
    x = float(tok)
    if not math.isfinite(x):
        errs.append(f'{what}: double 로 표현 못 하는 수 (inf) "{tok[:12]}..."')
        return None
    return x


def _vec(tok, errs, what):
    parts = tok.split(',')
    if len(parts) != 3:
        errs.append(f'{what}: x,y,z 아님 "{tok}"')
        return None
    v = [_num(p, errs, what) for p in parts]
    return None if None in v else v


def _dir(tok, errs, pol, what):
    v = _vec(tok, errs, what)
    if v is None:
        return
    if any(c < -1 or c > 1 for c in v):
        errs.append(f'{what}: 축이 [-1,1] 밖 "{tok}"')
        return
    n = math.sqrt(sum(c * c for c in v))
    if n < 1e-6:
        errs.append(f'{what}: 영벡터')
    elif abs(n - 1) > 1e-3:
        pol.add('unnorm')


def _color(tok, errs, what):
    parts = tok.split(',')
    if len(parts) != 3 or not all(INT.match(p) for p in parts):
        errs.append(f'{what}: R,G,B 정수 아님 "{tok}"')
        return
    if any(int(p) > 255 for p in parts):
        errs.append(f'{what}: 색이 255 초과 "{tok}"')


def _range(tok, lo, hi, errs, what, lo_open=False):
    x = _num(tok, errs, what)
    if x is None:
        return
    if (x <= lo if lo_open else x < lo) or (hi is not None and x > hi):
        errs.append(f'{what}: 범위 밖 {tok}')


def check_line(tokens, errs, pol, lineno):
    ident = tokens[0]
    w = f'{lineno}행 {ident}'
    if ident not in COUNT:
        errs.append(f'{lineno}행: 모르는 식별자 "{ident}"')
        return None
    if len(tokens) != COUNT[ident]:
        errs.append(f'{w}: 칸 수 {len(tokens)} (필요 {COUNT[ident]})')
        return ident
    t = tokens
    if ident == 'A':
        _range(t[1], 0, 1, errs, w + ' ratio')
        _color(t[2], errs, w)
    elif ident == 'C':
        _vec(t[1], errs, w + ' 위치')
        _dir(t[2], errs, pol, w + ' 방향')
        _range(t[3], 0, 180, errs, w + ' fov')
    elif ident == 'L':
        _vec(t[1], errs, w + ' 위치')
        _range(t[2], 0, 1, errs, w + ' brightness')
        _color(t[3], errs, w)
    elif ident == 'sp':
        _vec(t[1], errs, w + ' 중심')
        _range(t[2], 0, None, errs, w + ' 지름', lo_open=True)
        _color(t[3], errs, w)
    elif ident == 'pl':
        _vec(t[1], errs, w + ' 점')
        _dir(t[2], errs, pol, w + ' 법선')
        _color(t[3], errs, w)
    elif ident == 'cy':
        _vec(t[1], errs, w + ' 중심')
        _dir(t[2], errs, pol, w + ' 축')
        _range(t[3], 0, None, errs, w + ' 지름', lo_open=True)
        _range(t[4], 0, None, errs, w + ' 높이', lo_open=True)
        _color(t[5], errs, w)
    return ident


def classify(data):
    """bytes → (verdict, [위반 사유], {정책 플래그})"""
    text = data.decode('latin-1')
    errs, pol, seen = [], set(), {}
    for i, raw in enumerate(text.split('\n'), 1):
        line = raw[:-1] if raw.endswith('\r') else raw
        if line.strip() == '':
            continue
        if line.lstrip().startswith('#'):
            pol.add('comment')
            continue
        if '\t' in line:
            pol.add('tab')
            line = line.replace('\t', ' ')
        tokens = [t for t in line.split(' ') if t]
        ident = check_line(tokens, errs, pol, i)
        if ident in ('A', 'C', 'L'):
            seen[ident] = seen.get(ident, 0) + 1
    for k in ('A', 'C', 'L'):
        if seen.get(k, 0) == 0:
            errs.append(f'{k} 없음')
        elif seen[k] > 1:
            errs.append(f'{k} {seen[k]}번 (한 번만 가능)')
    if errs:
        return 'INVALID', errs, pol
    return ('POLICY' if pol else 'VALID'), errs, pol


# ---------------------------------------------------------------------------
# 렌더러를 더 많은 씬으로 돌려보기 위한 '변환본'
#   원본 판정과는 별개. 파서 검사는 항상 원본으로 한다
#   주석/탭 제거, 옛 subject 의 c/l → C/L, R·sq·tr·co 같은 보너스/옛 요소 줄 삭제,
#   광원·카메라는 첫 번째만, 옛 순서(cy 위치 축 색 지름 높이)는 새 순서로
# ---------------------------------------------------------------------------
def convert(data):
    text = data.decode('latin-1')
    out, have = [], set()
    dropped = set()
    for raw in text.split('\n'):
        line = raw.rstrip('\r').replace('\t', ' ')
        if line.strip() == '' or line.lstrip().startswith('#'):
            continue
        t = [x for x in line.split(' ') if x]
        ident = {'c': 'C', 'l': 'L'}.get(t[0], t[0])
        if ident not in COUNT:
            dropped.add(t[0])
            continue
        if ident in ('A', 'C', 'L'):
            if ident in have:
                dropped.add(ident + '(중복)')
                continue
            have.add(ident)
        if ident in ('pl', 'cy') and len(t) > 3 and t[3].count(',') == 2 and ident == 'cy':
            # 옛 순서: cy 위치 축 색 지름 높이
            t = [t[0], t[1], t[2]] + t[4:6] + [t[3]] + t[6:]
        n = COUNT[ident]
        if len(t) > n:
            dropped.add(ident + '(추가 인자)')
        t = [ident] + t[1:n]
        out.append(' '.join(t))
    return ('\n'.join(out) + '\n').encode('latin-1'), sorted(dropped)
