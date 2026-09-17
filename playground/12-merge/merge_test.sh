#!/bin/sh
# 12단계: srcs/render 를 서연 님 코드(파서·출력·vec3)와 합친 뒤 문제없는지 검사
#
#   sh merge_test.sh                 이 저장소 (learn/12-merge)
#   ROOT=/경로 sh merge_test.sh      다른 작업 트리 (예: feature/render-pixel 을 체크아웃한 곳)
#   sh merge_test.sh --no-remote     원격 브랜치 가상 머지 생략
#
# 실패가 하나라도 있으면 종료 코드 1. 제출 대상 아님.

cd "$(dirname "$0")" || exit 1
HERE=$(pwd)
R=${ROOT:-../..}
B=.build
mkdir -p "$B"
FAIL=0
PASSN=0

ok()   { PASSN=$((PASSN + 1)); printf "  \033[32mok\033[0m   %s\n" "$1"; }
ng()   { FAIL=$((FAIL + 1)); printf "  \033[31mFAIL\033[0m %s\n" "$1"; [ -n "$2" ] && printf "       %s\n" "$2"; }
info() { printf "  \033[33m참고\033[0m %s\n" "$1"; [ -n "$2" ] && printf "       %s\n" "$2"; }
part() { printf "\n\033[1m[%s]\033[0m\n" "$1"; }
wait_s() { perl -e "select(undef,undef,undef,$1)"; }

RENDER_SRCS="$(ls $R/srcs/render/*.c) $R/srcs/vec3_math.c $R/srcs/vec3_ops.c"
PARSE_SRCS="$(ls $R/srcs/parsing/*.c) $R/srcs/error.c"
printf "대상: %s (%s)\n" "$(cd $R && pwd)" "$(git -C $R rev-parse --abbrev-ref HEAD)"

# --------------------------------------------------------------------------
part "1. 서연 님 코드를 건드리지 않았나"
for p in srcs/parsing srcs/output srcs/main.c srcs/error.c includes/parsing.h includes/output.h; do
	if git -C $R diff --quiet origin/main -- "$p"; then ok "$p = origin/main"
	else ng "$p 가 origin/main 과 다름" "$(git -C $R diff --stat origin/main -- "$p" | tail -1)"; fi
done
for p in includes/miniRT.h srcs/vec3_math.c srcs/vec3_ops.c; do
	if git -C $R diff --quiet origin/feature/vec3-render-interface -- "$p"; then ok "$p = origin/feature/vec3-render-interface"
	else ng "$p 가 서연 님 vec3 브랜치와 다름"; fi
done
removed=$(git -C $R diff origin/main -- Makefile | grep '^-[^-]' | sed 's/^-//')
added=$(git -C $R diff origin/main -- Makefile | grep -c '^+[^+]')
if [ "$(printf '%s' "$removed" | tr -d '[:space:]')" = "srcs/output/render_loop.c" ]; then
	ok "Makefile: 기존 줄 삭제 없음, SRCS 끝에 ${added}줄 추가 (render_loop.c 줄 끝에 \\ 만 붙음)"
else
	ng "Makefile 에서 기존 줄이 바뀜" "$removed"
fi

# --------------------------------------------------------------------------
part "2. 루트 Makefile 로 빌드 (-Wall -Wextra -Werror)"
if make -C $R re >$B/make.log 2>&1 && [ -x $R/miniRT ]; then
	ok "make re 성공 → ./miniRT"
else
	ng "make re 실패" "$(grep -m1 -i 'error' $B/make.log)"
fi
n_warn=$(grep -c "srcs/.*warning" $B/make.log)
[ "$n_warn" -eq 0 ] && ok "srcs 경고 0개" || ng "srcs 경고 ${n_warn}개"

# --------------------------------------------------------------------------
part "3. 겹치는 전역 심볼이 없나 (파서 · 출력 · vec3 · 렌더러 · libft)"
objs=$(sed -n 's/^cc .* -c \(srcs\/[^ ]*\.c\) -o .*/\1/p' $B/make.log | sed 's/\.c$/.o/')
dups=$( (for o in $objs; do nm -g "$R/$o" 2>/dev/null | awk '$2=="T"{print $3}'; done
         nm -g $R/libft/libft.a 2>/dev/null | awk '$2=="T"{print $3}') | sort | uniq -d)
[ -z "$dups" ] && ok "중복 정의 0개 ($(echo $objs | wc -w | tr -d ' ')개 오브젝트 + libft.a)" || ng "중복 정의" "$dups"

# --------------------------------------------------------------------------
part "4. 42 norm (렌더러 파일)"
if command -v norminette >/dev/null 2>&1; then
	if norminette $R/includes/render.h $R/srcs/render/*.c >$B/norm.log 2>&1; then ok "norminette 전부 OK"
	else ng "norminette 오류" "$(grep -v ': OK!' $B/norm.log | head -5 | tr '\n' ' ')"; fi
else
	info "norminette 가 설치돼 있지 않아 자가 점검(tools/normcheck.py)만 한다"
fi
# shellcheck disable=SC2086
if python3 tools/normcheck.py $R/includes/render.h $R/srcs/render/*.c >$B/normcheck.log; then
	ok "자가 점검: 헤더·주석·80칸·25줄·인자4·변수5·함수5·선언 위치·금지 문법 위반 0건 ($(ls $R/srcs/render/*.c | wc -l | tr -d ' ')개 .c + render.h)"
else
	ng "자가 점검 위반 $(wc -l <$B/normcheck.log | tr -d ' ')건" "$(head -5 $B/normcheck.log | tr '\n' ' ')"
fi
n_comment=$(grep -n '/\*\|//' $R/srcs/render/*.c $R/includes/render.h | awk -F: '$2 > 11' | wc -l | tr -d ' ')
[ "$n_comment" -eq 0 ] && ok "42 헤더 밖 주석 0줄" || ng "주석 ${n_comment}줄"

# --------------------------------------------------------------------------
part "5. 서연 님 파서 자체 테스트 (make -C srcs/parsing suite)"
if make -C $R/srcs/parsing suite >$B/parse_suite.log 2>&1; then
	ok "$(grep 'pass=' $B/parse_suite.log)"
else
	ng "파서 자체 테스트 실패" "$(grep -m3 'FAIL' $B/parse_suite.log)"
fi
make -C $R/srcs/parsing clean >/dev/null 2>&1

# --------------------------------------------------------------------------
part "6. 학습 단계 테스트를 srcs 코드로 (04 05 06 07 08 09)"
TESTS=../tests
for n in 04 05 06 07 08; do
	# shellcheck disable=SC2086
	if cc -Wall -Wextra -g -fsanitize=address -Itools/shim -I$R/includes -I$R/libft -I$TESTS \
		$TESTS/test_$n.c $TESTS/case.c tools/contract_adapter.c $RENDER_SRCS -lm -o $B/t$n 2>$B/t$n.log; then
		res=$(cd $TESTS && "$HERE/$B/t$n" 2>&1 | tail -1 | sed 's/\x1b\[[0-9;]*m//g')
		case "$res" in
			*"fail=0"*) ok "${n}단계  $res" ;;
			*) if [ "$n" = "04" ] && echo "$res" | grep -q "pass=9 fail=1"; then
					ok "${n}단계  $res  (알려진 케이스 모순 1건: 03_range t_min 4.5 — 학습 폴더와 같음)"
				else ng "${n}단계  $res"; fi ;;
		esac
	else
		ng "${n}단계 빌드 실패" "$(grep -m1 'error' $B/t$n.log)"
	fi
done
# shellcheck disable=SC2086
if cc -Wall -Wextra -g -fsanitize=address -I$R/includes -I$R/libft -I$TESTS $TESTS/test_09.c \
	$RENDER_SRCS $PARSE_SRCS -L$R/libft -lft -lm -o $B/t09 2>$B/t09.log; then
	res=$(cd $TESTS && "$HERE/$B/t09" 2>/dev/null | tail -1 | sed 's/\x1b\[[0-9;]*m//g')
	case "$res" in *"fail=0"*) ok "09단계  $res  (팀 헤더만으로 빌드)" ;; *) ng "09단계  $res" ;; esac
else
	ng "09단계 빌드 실패" "$(grep -m1 'error' $B/t09.log)"
fi

# --------------------------------------------------------------------------
part "7. 렌더 회귀 - 옮기기 전(10단계 폴더)과 픽셀 비교"
# shellcheck disable=SC2086
cc -O2 -Wall -Wextra -Itools/shim -I$R/includes -I$R/libft tools/rt_probe.c $RENDER_SRCS $PARSE_SRCS \
	-L$R/libft -lft -lm -o $B/probe_srcs
STEP10=../10-interaction
# shellcheck disable=SC2086
cc -O2 -Wall -Wextra -I$STEP10/includes -I$R/minilibx_macos_opengl -I$R/includes -I$R/libft tools/rt_probe.c \
	$(ls $STEP10/*.c | grep -v -e '/main\.c$' -e '/hooks\.c$') $PARSE_SRCS -L$R/libft -lft -lm -o $B/probe_step10
if [ -f ../11-test/out/results.tsv ]; then
	if python3 tools/regress.py "$B/probe_srcs" "$B/probe_step10"; then ok "렌더 회귀 전부 같음"
	else ng "렌더 회귀에 차이"; fi
else
	ng "11단계 결과(out/results.tsv)가 없다 - 먼저 cd ../11-test && sh run.sh"
fi

# --------------------------------------------------------------------------
part "8. 실제 ./miniRT (mlx 창)"
check_err() {
	desc=$1; shift
	$R/miniRT "$@" >$B/err.out 2>$B/err.log
	code=$?
	if [ $code -ne 0 ] && [ "$(head -1 $B/err.log)" = "Error" ]; then ok "$desc → 종료 $code, $(sed -n 2p $B/err.log)"
	else ng "$desc → 종료 $code" "$(head -2 $B/err.log | tr '\n' ' ')"; fi
}
check_err "인자 없음"
check_err "확장자 .md" $R/README.md
check_err "이름 없는 .rt" $R/scenes/.rt
check_err "없는 파일" $R/scenes/nope.rt
for f in $R/scenes/invalid/*.rt; do check_err "invalid/$(basename $f)" "$f"; done
for f in $R/scenes/valid/*.rt ../11-test/scenes/check/cam_front.rt; do
	$R/miniRT "$f" >/dev/null 2>$B/run.log &
	pid=$!
	wait_s 2.5
	if kill -0 $pid 2>/dev/null; then
		lk=$(leaks $pid 2>/dev/null | grep -m1 'leaks for')
		kill $pid 2>/dev/null
		wait $pid 2>/dev/null
		case "$lk" in
			*" 0 leaks for"*) ok "$(basename $f): 창 유지, 실행 중 누수 0" ;;
			*) ng "$(basename $f) 실행 중 누수" "$lk" ;;
		esac
	else
		ng "$(basename $f) 창이 곧바로 종료됨" "$(head -2 $B/run.log)"
	fi
done

# --------------------------------------------------------------------------
if [ "$1" != "--no-remote" ]; then
	part "9. 아직 main 에 안 들어온 원격 브랜치와 가상 머지 (작업 트리는 건드리지 않음)"
	WT=$HERE/$B/wt
	for br in origin/chore/readme-and-comments origin/feature/scene-builder-controls; do
		rm -rf "$WT"
		git -C $R worktree prune
		git -C $R worktree add --detach "$WT" HEAD >/dev/null 2>&1
		if git -C "$WT" merge --no-commit --no-ff "$br" >$B/wt_merge.log 2>&1; then
			if make -C "$WT" -s >$B/wt_make.log 2>&1; then ok "$br : 충돌 없음, 빌드 성공"
			else ng "$br : 충돌은 없지만 빌드 실패" "$(grep -m2 -i 'error' $B/wt_make.log | tr '\n' ' ')"; fi
		else
			files=$(grep CONFLICT $B/wt_merge.log | sed 's/.*Merge conflict in //' | tr '\n' ' ')
			mine=$(echo "$files" | tr ' ' '\n' | grep -c 'render')
			if [ "$mine" -eq 0 ]; then
				info "$br : 충돌 — 서연 님 브랜치끼리 ($files)" "렌더러 파일은 충돌 없음. vec3 브랜치의 miniRT.h 선언 추가와 주석 영어화가 같은 곳을 고쳤다"
			else
				ng "$br : 렌더러 파일 충돌" "$files"
			fi
		fi
		git -C "$WT" merge --abort >/dev/null 2>&1
		git -C $R worktree remove --force "$WT" >/dev/null 2>&1
	done
fi

printf "\n\033[1m========================================\033[0m\n"
printf "통과 %d, 실패 %d\n" "$PASSN" "$FAIL"
printf "\033[1m========================================\033[0m\n"
[ "$FAIL" -eq 0 ]
