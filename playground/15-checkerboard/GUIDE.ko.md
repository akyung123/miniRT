# 15단계 — 체크무늬 색상 왜곡

**브랜치**: `learn/15-checkerboard` (`learn/12-merge`에서 분기)

구·평면·원기둥에 선택적으로 체크무늬 albedo를 입힙니다. 기존 `.rt` 줄은 그대로 두고, 무늬가 필요할 때만 토큰을 덧붙입니다.

```
sp <center> <diameter> <R,G,B> [ck <scale> <R,G,B>]
pl <point> <normal> <R,G,B> [ck <scale> <R,G,B>]
cy <center> <axis> <diameter> <height> <R,G,B> [ck <scale> <R,G,B>]
```

`ck` 뒤의 색이 홀수 칸, 원래 색이 짝수 칸입니다.

```sh
cd playground/15-checkerboard
make
./miniRT scenes/checkerboard.rt
```

## UV

| 도형 | u, v |
|---|---|
| 평면 | 법선에 수직인 두 축으로 투영 |
| 구 | `atan2(z,x)·r`, `acos(y)·r` |
| 원기둥 | 둘레 각도·축 높이 |

교차점에서 albedo만 바꿉니다. 조명 공식은 mandatory와 같습니다.
