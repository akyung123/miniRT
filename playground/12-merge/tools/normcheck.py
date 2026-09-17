"""norminette 가 없는 환경에서 쓰는 42 norm 자가 점검 - 제출 대상 아님, norminette 대신이 아니다

  python3 tools/normcheck.py 파일...

확인하는 규칙 (norminette v3 의 주요 오류 이름을 괄호에)
  42 헤더 (INVALID_HEADER)              헤더 밖 주석 (COMMENT_ON_INSTR / 요청: 주석 전부 제거)
  한 줄 80칸, 탭은 4칸 (LINE_TOO_LONG)  줄 끝 공백, 공백 들여쓰기 (SPC_BEFORE_NL / MIXED_SPACE_TAB)
  빈 줄 연속, 파일 끝 개행 (CONSECUTIVE_NEWLINES)
  함수 25줄 (TOO_MANY_LINES)            인자 4개 (TOO_MANY_ARGS)
  변수 선언 5개 (TOO_MANY_VARS_FUNC)    파일당 함수 5개 (TOO_MANY_FUNCS)
  선언 뒤 빈 줄 하나, 본문 안 빈 줄 없음 (NL_AFTER_VAR / EMPTY_LINE_FUNCTION)
  선언과 초기화 분리, 한 줄 한 선언 (DECL_ASSIGN_LINE / MULT_DECL_LINE)
  변수 이름 열 맞춤 (MISALIGNED_VAR_DECL) 줄 끝 연산자 (EOL_OPERATOR)
  for / do / switch / case / goto / 삼항 (FORBIDDEN_CS / TERNARY)
  return 뒤 괄호 (RETURN_PARENTHESIS)   헤더 가드 이름 (HEADER_PROT_NAME)
  복합 리터럴 (t_vec3){...} — norminette 가 해석하지 못하는 경우가 있어 피한다
"""
import re
import sys

ERR = []


def col(s):
    n = 0
    for ch in s:
        n = (n // 4 + 1) * 4 if ch == '\t' else n + 1
    return n


def err(f, ln, msg):
    ERR.append(f'{f}:{ln}: {msg}')


def strip_strings(s):
    return re.sub(r'"(\\.|[^"\\])*"', '""', re.sub(r"'(\\.|[^'\\])'", "''", s))


def check(path):
    text = open(path, encoding='utf-8').read()
    L = text.split('\n')
    if not text.endswith('\n'):
        err(path, len(L), '파일 끝 개행 없음')
    if text.endswith('\n\n'):
        err(path, len(L), '파일 끝 빈 줄')
    L = L[:-1]
    hdr = L[:11]
    if len(hdr) < 11 or not all(h.startswith('/*') and h.endswith('*/') and len(h) == 80 for h in hdr) \
            or ':::      ::::::::' not in hdr[2] or 'By: ' not in hdr[5]:
        err(path, 1, '42 헤더 없음 또는 형식 다름')
        start = 0
    else:
        start = 11
    for i, line in enumerate(L, 1):
        if col(line) > 80:
            err(path, i, f'{col(line)}칸 (80 초과)')
        if line != line.rstrip():
            err(path, i, '줄 끝 공백')
        if re.match(r'^ +\S', line):
            err(path, i, '공백으로 들여씀')
        if i > 1 and line == '' and L[i - 2] == '':
            err(path, i, '빈 줄 연속')
        if i > start:
            s = strip_strings(line)
            if '/*' in s or '//' in s or s.strip().startswith('*/'):
                err(path, i, '주석')
            if re.search(r'\b(for|do|switch|case|goto)\b', s):
                err(path, i, '금지된 제어문')
            if '?' in s:
                err(path, i, '삼항 연산자')
            if re.search(r'\breturn\b(?!\s*\(|\s*;)', s):
                err(path, i, 'return 뒤에 괄호 없음')
            if re.search(r'(&&|\|\||[^-]\+|[^-]-|[^/]\*|/|%|==|!=|<=|>=|<|>)\s*$', s) and not s.rstrip().endswith('*/') \
                    and not s.strip().startswith('#') and not re.search(r'\(\s*$|,\s*$', s):
                if not re.search(r'[a-z_]+\s*\*$', s.rstrip()):
                    err(path, i, '줄 끝에 연산자 (다음 줄 앞으로)')
            if re.search(r'\(t_[a-z0-9_]+\)\s*\{', s):
                err(path, i, '복합 리터럴')
    if path.endswith('.h'):
        guard = re.sub(r'[^A-Z0-9]', '_', path.split('/')[-1].upper())
        body = '\n'.join(L[start:])
        if f'#ifndef {guard}' not in body or f'# define {guard}' not in body:
            err(path, start + 1, f'헤더 가드가 {guard} 가 아님')
        for i, line in enumerate(L[start:], start + 1):
            if re.match(r'^#\s*(define|include)', line) and not line.startswith('#ifndef'):
                if not line.startswith('# '):
                    err(path, i, '가드 안 전처리기는 "# define" 처럼 들여써야 함')
        return
    # ---------------------------------------------------------------- 함수
    funcs = 0
    i = start
    while i < len(L):
        line = L[i]
        if line and not line[0].isspace() and line[0] not in '#{}' and '(' in line and not line.rstrip().endswith(';') \
                and not line.startswith('typedef'):
            sig = line
            j = i
            while not L[j].rstrip().endswith(')'):
                j += 1
                sig += ' ' + L[j].strip()
            if j + 1 >= len(L) or L[j + 1] != '{':
                i += 1
                continue
            funcs += 1
            fname = re.search(r'([a-z_0-9]+)\s*\(', sig).group(1)
            params = sig[sig.index('(') + 1:sig.rindex(')')].strip()
            nargs = 0 if params in ('', 'void') else params.count(',') + 1
            if nargs > 4:
                err(path, i + 1, f'{fname}: 인자 {nargs}개')
            b0 = j + 2
            depth = 1
            k = j + 1
            while True:
                k += 1
                if L[k].startswith('}') and L[k].strip() == '}' and not L[k][0].isspace():
                    break
            body = L[b0:k]
            if len(body) > 25:
                err(path, i + 1, f'{fname}: 본문 {len(body)}줄')
            decls = []
            m = 0
            decl_re = re.compile(r'^\t(static\s+)?(const\s+)?(unsigned\s+)?[a-z_][a-z_0-9]*\t+\**[a-z_][a-z_0-9]*(\[\d+\])?;$')
            while m < len(body) and decl_re.match(body[m]):
                decls.append(body[m])
                m += 1
            if len(decls) > 5:
                err(path, i + 1, f'{fname}: 변수 {len(decls)}개')
            if decls:
                if m >= len(body) or body[m] != '':
                    err(path, b0 + m + 1, f'{fname}: 선언 뒤 빈 줄 없음')
                else:
                    m += 1
                names = {col(d[:len(d) - len(d.lstrip('\t').split('\t', 1)[1].lstrip('\t')) if '\t' in d.lstrip('\t') else 0])
                         for d in decls}
                cols = set()
                for d in decls:
                    s = d.lstrip('\t')
                    head, tail = s.split('\t', 1)
                    prefix = d[:len(d) - len(tail.lstrip('\t'))]
                    cols.add(col(prefix))
                if len(cols) > 1:
                    err(path, b0 + 1, f'{fname}: 변수 이름 열이 안 맞음 {sorted(cols)}')
                del names
            for n, bl in enumerate(body[m:], b0 + m + 1):
                if bl == '':
                    err(path, n, f'{fname}: 본문 안 빈 줄')
                s = strip_strings(bl)
                if re.match(r'^\t+(static\s+)?[a-z_][a-z_0-9]*\t+\**[a-z_]', bl) and ';' in bl and '(' not in bl \
                        and not re.match(r'^\t+(return|if|while|else)\b', bl):
                    err(path, n, f'{fname}: 선언이 본문 중간에 있음')
                if re.search(r'^\t+(static\s+)?[a-z_][a-z_0-9]*[ \t]+\**[a-z_][a-z_0-9]*\s*=', bl) \
                        and not re.match(r'^\t+(return|if|while|else)\b', bl):
                    err(path, n, f'{fname}: 선언과 동시에 초기화')
            i = k + 1
            continue
        i += 1
    if funcs > 5:
        err(path, 1, f'함수 {funcs}개 (5 초과)')


def main():
    for p in sys.argv[1:]:
        check(p)
    for e in ERR:
        print('   ', e)
    return 1 if ERR else 0


if __name__ == '__main__':
    sys.exit(main())
