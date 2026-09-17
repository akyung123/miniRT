# 18단계 — 절차적 범프 매핑

**브랜치**: `learn/18-bump-map` (`learn/12-merge`에서 분기, 14–17에 쌓지 않음)

구·평면·원기둥에 선택적으로 높이 함수로 법선을 흔듭니다. 알베도는 그대로이고, 조명만 울퉁불퉁해 보입니다. 기존 `.rt` 줄은 그대로 두고, 무늬가 필요할 때만 토큰을 덧붙입니다.

```
sp <center> <diameter> <R,G,B> [bp <freq> <strength>]
pl <point> <normal> <R,G,B> [bp <freq> <strength>]
cy <center> <axis> <diameter> <height> <R,G,B> [bp <freq> <strength>]
```

`freq` 는 물결 밀도(> 0), `strength` 는 접선 공간에서 기울기 크기(≥ 0)입니다.

```sh
cd playground/18-bump-map
make dump_ppm
./dump_ppm scenes/bump_plane.rt > bump_plane.ppm
./miniRT scenes/bump_plane.rt          # mlx 창 (macOS)
```

## 수식

기하 법선 `N` 을 구한 뒤, **뒷면이면 먼저 뒤집고**, 그다음에 UV와 TBN으로 흔듭니다.

높이 함수 (계란판):

```
h(u,v) = sin(freq·u) · sin(freq·v)
hu     = cos(freq·u) · sin(freq·v)
hv     = sin(freq·u) · cos(freq·v)
N'     = normalize(N − strength · (hu T + hv B))
```

`hu`/`hv` 에서 연쇄법칙의 추가 `freq` 배율은 빼 두었습니다. `strength` 가 주파수와 독립인 0~1 근처 슬라이더가 됩니다.

TBN 은 UV가 증가하는 방향입니다. 뒷면을 뒤집은 뒤에는 `T×B` 가 현재 `N` 과 같은 반구를 보도록 `T`,`B` 도 함께 부호를 바꿉니다.

## UV

| 도형 | u, v | T, B |
|---|---|---|
| 평면 | 법선에 수직인 두 축으로 투영 | 그 두 축 (정규직교) |
| 구 | `atan2(z,x)·r`, `acos(y)·r` | 경도·위도 방향 |
| 원기둥 측면 | 둘레 호길이, 축 높이 | 둘레, 축 |
| 원기둥 뚜껑 | 뚜껑 법선 기준 평면과 같음 | 뚜껑 TBN |

교차점의 **법선만** 바꿉니다. 그림자와 알베도 공식은 mandatory와 같습니다.

## 데모 씬

| 파일 | 보는 것 |
|---|---|
| `scenes/bump_plane.rt` | 옆빛으로 찍은 계란판 바닥 |
| `scenes/bump_sphere.rt` | 왼쪽 범프 구 / 오른쪽 매끈 구 |
| `scenes/bump_shapes.rt` | 평면·구·원기둥 모두 `bp` |

`render_pixel(t_scene *, int, int)` 경계는 그대로입니다.
