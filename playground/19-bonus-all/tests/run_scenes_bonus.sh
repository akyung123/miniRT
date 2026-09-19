#!/bin/sh
# 보너스 파서 회귀 테스트. mandatory 씬 + 보너스 문법 씬을 전부 돌린다.
# (보너스는 mandatory 씬도 전부 받아야 한다.)
# 사용법: make dump_ppm_bonus 뒤 이 스크립트 실행. mlx 불필요.

cd "$(dirname "$0")/.." || exit 1
BIN=tests/parse_test_bonus

if [ ! -x "$BIN" ]; then
	echo "빌드 필요: cc ... tests/parse_test_bonus.c ... -o tests/parse_test_bonus"
	exit 1
fi

pass=0
fail=0

for f in scenes/valid/*.rt scenes/bonus/valid/*.rt scenes/bonus/*.rt; do
	[ -f "$f" ] || continue
	if "$BIN" "$f" >/dev/null 2>&1; then
		echo "OK   (valid)   $f"
		pass=$((pass + 1))
	else
		echo "FAIL (valid)   $f  <- 성공해야 하는데 실패함"
		fail=$((fail + 1))
	fi
done

for f in scenes/invalid/*.rt scenes/bonus/invalid/*.rt; do
	[ -f "$f" ] || continue
	if "$BIN" "$f" >/dev/null 2>&1; then
		echo "FAIL (invalid) $f  <- 실패해야 하는데 성공함"
		fail=$((fail + 1))
	else
		echo "OK   (invalid) $f"
		pass=$((pass + 1))
	fi
done

echo "---"
echo "pass=$pass fail=$fail"
[ "$fail" -eq 0 ]
