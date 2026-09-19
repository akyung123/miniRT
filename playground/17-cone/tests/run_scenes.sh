#!/bin/sh
# scenes/valid, scenes/invalid를 전부 돌려서 parse_scene이 기대한 대로
# 성공/실패하는지 확인하는 회귀 테스트 스크립트. 제출 대상 아님, mlx 불필요.
# 사용법: srcs/parsing 에서 `make suite` 하거나, 이 스크립트를 직접 실행.

cd "$(dirname "$0")/.." || exit 1
BIN=tests/parse_test

if [ ! -x "$BIN" ]; then
	echo "빌드 필요: (cd srcs/parsing && make test)"
	exit 1
fi

pass=0
fail=0

for f in scenes/valid/*.rt; do
	if "$BIN" "$f" >/dev/null 2>&1; then
		echo "OK   (valid)   $f"
		pass=$((pass + 1))
	else
		echo "FAIL (valid)   $f  <- 성공해야 하는데 실패함"
		fail=$((fail + 1))
	fi
done

for f in scenes/invalid/*.rt; do
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
