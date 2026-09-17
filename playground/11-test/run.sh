#!/bin/sh
# 11단계 테스트: 웹에서 모은 씬 + 확인용 씬으로 파서/렌더러를 한꺼번에 검사
#
#   sh run.sh              전부 (leaks, 시간 측정 포함 - 몇 분 걸림)
#   sh run.sh --no-leaks   누수 검사 생략
#   sh run.sh --no-time    루트 Makefile 플래그 시간 측정 생략
#
# 결과: out/summary.txt, out/results.tsv, out/sheets/sheet_NN.png (+ index.txt)
cd "$(dirname "$0")" || exit 1
sh build.sh || exit 1

printf "\n\033[1m[1/3] 기존 9단계 인수 테스트\033[0m\n"
sh ../tests/run.sh 9 | tail -4

printf "\n\033[1m[2/3] 10단계 이동 헤드리스 검사\033[0m\n"
.build/check_move scenes/check/cam_front.rt | tail -2

printf "\n\033[1m[3/3] 씬 일괄 검사\033[0m\n"
python3 tools/suite.py "$@"
