# 14단계 — Phong 스펙큘러 반사

**브랜치**: `learn/14-phong` (`learn/12-merge`에서 분기)

램버트 난반사 위에, 시선이 거울 반사와 맞을 때 반짝이는 점을 더합니다. `.rt` 문법은 그대로입니다.

```sh
cd playground/14-phong
make
./miniRT scenes/phong.rt
```

창 없이 이미지 파일만 뽑으려면 `make dump_ppm && ./dump_ppm scenes/phong.rt > phong.ppm`.

## 수식

```
L = normalize(light_pos − P)
N = hit normal
V = normalize(camera_pos − P)
R = 2·dot(N,L)·N − L
specular = light_color · brightness · ks · max(dot(R,V), 0)^shininess
color = clamp(ambient + diffuse + specular, 0, 1)
```

스펙큘러에는 **물체색을 곱하지 않습니다.** 하이라이트는 광원 색입니다. 그림자이거나 `N·L ≤ 0` 이면 ambient만.

| 상수 | 값 |
|---|---|
| `ks` | `0.4` (`SPECULAR_KS`) |
| `shininess` | `32` (`SPECULAR_SHININESS`) |

## 바뀐 파일

| 파일 | 내용 |
|---|---|
| `srcs/render/lighting.c` | `apply_specular` |
| `srcs/render/render_pixel.c` | `V` 를 lighting에 전달 |
| `includes/render.h` | 시그니처와 상수 |

`render_pixel(t_scene *, int, int)` 경계는 그대로입니다.
