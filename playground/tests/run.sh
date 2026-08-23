#!/bin/sh
# 학습 단계 테스트 러너. 제출 대상 아님.
#
# 각 단계는 cases/<단계>/ 안의 .case 파일들로 평가합니다.
# "그 단계에 주어진 케이스 파일이 전부 통과 = 그 단계 완료" 가 합격 기준입니다.
# 9단계만 예외로, cases/ 대신 실제 scenes/*.rt 를 그대로 씁니다.
#
#   sh run.sh          전체 단계
#   sh run.sh 4        4단계만
#
# 아직 구현 안 한 단계는 링크가 안 되므로 SKIP 으로 넘어갑니다.
# 실패가 하나라도 있으면 종료 코드 1.

cd "$(dirname "$0")" || exit 1

CC=cc
CFLAGS="-Wall -Wextra -g -fsanitize=address"
MLX=../../minilibx_macos_opengl
BUILD=.build
mkdir -p "$BUILD"

TOTAL_FAIL=0
RAN=0
SKIPPED=""

# step_dir 를 단계 번호로 찾는다 (01-mlx, 02-color ... 이름이 달라도 됨)
find_dir()
{
	for d in ../"$1"-*; do
		[ -d "$d" ] && echo "$d" && return 0
	done
	return 1
}

run_step()
{
	n="$1"
	dir=$(find_dir "$n")
	test_file="test_$n.c"

	if [ ! -f "$test_file" ]; then
		return 0
	fi
	if [ -z "$dir" ]; then
		SKIPPED="$SKIPPED $n(폴더없음)"
		return 0
	fi

	# 그 단계의 소스 전부, main.c 만 제외 (테스트가 자기 main 을 가짐)
	srcs=""
	for f in "$dir"/*.c; do
		case "$f" in
			*/main.c) ;;
			*) [ -f "$f" ] && srcs="$srcs $f" ;;
		esac
	done

	inc="-I. -I$dir/includes -I$dir -I$MLX -I../../includes -I../../libft"
	extra=""
	# 9단계는 진짜 파서와 libft 를 함께 링크한다
	if [ "$n" = "09" ]; then
		if [ ! -f ../../libft/libft.a ]; then
			make -C ../../libft >/dev/null 2>&1
		fi
		extra="../../srcs/parsing/parse_scene.c ../../srcs/parsing/parse_elements.c \
../../srcs/parsing/parse_numbers.c ../../srcs/parsing/parse_objects.c \
../../srcs/parsing/parse_utils.c ../../srcs/parsing/free_scene.c \
../../srcs/error.c -L../../libft -lft"
	fi

	out="$BUILD/t$n"
	log="$BUILD/t$n.log"
	# shellcheck disable=SC2086
	if ! $CC $CFLAGS $inc "$test_file" case.c $srcs $extra -lm -o "$out" >"$log" 2>&1; then
		reason=$(grep -m1 -i "undefined\|no such file\|error:" "$log" \
			| cut -c1-70)
		SKIPPED="$SKIPPED $n"
		printf "\n\033[33mSKIP\033[0m %s단계 - 아직 빌드 안 됨\n" "$n"
		printf "     %s\n" "$reason"
		printf "     (자세히: %s)\n" "$log"
		return 0
	fi
	RAN=$((RAN + 1))
	if ! "$out"; then
		TOTAL_FAIL=$((TOTAL_FAIL + 1))
	fi
}

if [ -n "$1" ]; then
	run_step "$(printf '%02d' "$1")"
else
	for n in 01 02 03 04 05 06 07 08 09; do
		run_step "$n"
	done
fi

printf "\n\033[1m========================================\033[0m\n"
printf "실행 %d단계, 실패 %d단계\n" "$RAN" "$TOTAL_FAIL"
[ -n "$SKIPPED" ] && printf "건너뜀:%s\n" "$SKIPPED"
printf "\033[1m========================================\033[0m\n"
[ "$TOTAL_FAIL" -eq 0 ]
