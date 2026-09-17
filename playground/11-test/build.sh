#!/bin/sh
# 11단계 도구 빌드. 렌더러는 10단계 폴더 소스를 그대로 쓴다 (main.c, hooks.c 는 mlx 전용이라 제외)
cd "$(dirname "$0")" || exit 1
R=../..
STEP=${STEP:-../10-interaction}
OUT=.build
mkdir -p "$OUT"
SRCS=$(ls "$STEP"/*.c | grep -v -e '/main\.c$' -e '/hooks\.c$')
# 파서 파일은 나열하지 않고 폴더째 (서연 파서에서 str_to_double.c 가 분리된 뒤 링크가 깨졌었다)
TEAM="$(ls $R/srcs/parsing/*.c) $R/srcs/error.c"
INC="-I$STEP/includes -I../tests -I$R/minilibx_macos_opengl -I$R/includes -I$R/libft"
LIB="-L$R/libft -lft -lm"
[ -f "$R/libft/libft.a" ] || make -C "$R/libft" >/dev/null
b() { # b <이름> <메인.c> <플래그...>
	name=$1; main=$2; shift 2
	# shellcheck disable=SC2086
	cc -Wall -Wextra -Werror "$@" $INC "$main" $SRCS $TEAM $LIB -o "$OUT/$name" || exit 1
}
b probe_asan tools/rt_probe.c -g -fsanitize=address   # 메모리 오류 잡기 (느림)
b probe_team tools/rt_probe.c -g                       # 루트 Makefile 과 같은 플래그 → 실제 체감 속도
b probe_o2   tools/rt_probe.c -O2                      # 이미지 뽑기용
b check_move tools/check_move.c -g -fsanitize=address
echo "built: $(ls $OUT | tr '\n' ' ')"
