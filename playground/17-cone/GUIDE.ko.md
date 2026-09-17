# 17단계 — 원뿔 (2차 곡면)

**브랜치**: `learn/17-cone` (`learn/12-merge`에서 분기)

원기둥과 같은 2차 곡선이지만, 반지름이 높이를 따라 0까지 줄어듭니다. 식별자는 `co`입니다.

```
co <center x,y,z> <axis x,y,z> <diameter> <height> <R,G,B>
```

`center`는 높이의 중점, `axis`는 꼭짓점 방향, `diameter`는 **밑면** 지름입니다. 꼭짓점은 `center + axis * height/2`, 밑면은 `center - axis * height/2`입니다. 밑면 원판(cap)도 닫습니다.

```sh
cd playground/17-cone
make
./miniRT scenes/cone.rt
./miniRT scenes/cone_plane.rt    # 밑면 cap + 바닥 그림자
./miniRT scenes/cone_tilted.rt   # 기울어진 축
./miniRT scenes/cone_multi.rt    # co 여러 개
```

## 교차

꼭짓점 `A`, 밑면 방향 단위벡터 `V`, 높이 `H`, 밑면 반지름 `R`:

```
cos²α = H² / (H² + R²)
((P − A) · V)² = |P − A|² · cos²α
0 < (P − A) · V ≤ H
```

측면 법선은 `∇f = mV − cos²α · (P − A)` 입니다 (`m = (P−A)·V`).
