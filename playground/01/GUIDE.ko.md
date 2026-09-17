# 보너스 01 — Phong 스펙큘러 반사

**브랜치**: `cursor/bonus-01-phong-specular-e49c` (`main`에서 분기)
**한 줄 요약**: 램버트 난반사 위에, 시선이 거울 반사와 맞을 때 반짝이는 점을 그린다.

이 폴더는 **문서 + 데모 씬**만 둔다. 렌더러 코드는 루트 `srcs/render/`가 본체다. `playground/13` 스냅샷과는 별개다.

---

## 0. Phong이 더하는 것

Mandatory 최종색은 이것뿐이다.

```
color = ambient + diffuse
```

구는 공처럼 보이지만 **플라스틱/금속의 하이라이트**가 없다. Phong은 거울처럼 튕긴 빛 `R`이 카메라 `V`와 얼마나 나란한지를 세 번째 항으로 넣는다.

```
color = ambient + diffuse + specular
```

그림자 안이거나 광원을 등지면 (`N·L ≤ 0` 또는 `in_shadow`) **ambient만** — mandatory와 같다.

---

## 1. 수식

교차점 `P`, 단위벡터:

```
L = normalize(light_pos − P)     /* 표면 → 광원 */
N = hit normal
V = normalize(camera_pos − P)    /* 표면 → 카메라 */
R = reflect(−L, N) = 2·dot(N,L)·N − L
```

```
specular = light_color · brightness · ks · max(dot(R,V), 0)^shininess
```

스펙큘러는 **물체색을 곱하지 않는다.** 하이라이트는 광원 색이다.

최종:

```
final = clamp(ambient + diffuse + specular, 0, 1)
```

### 고정 상수 (`.rt` 확장 없음)

| 이름 | 값 | 위치 |
|---|---|---|
| `ks` | `0.4` | `includes/render.h` `SPECULAR_KS` |
| `shininess` | `32` | `includes/render.h` `SPECULAR_SHININESS` |

`ks`가 클수록 하이라이트가 밝고, `shininess`가 클수록 점이 작고 날카롭다. 재질을 `.rt`에 쓰기 전까지 전 씬 공통이다.

---

## 2. 바뀐 파일

| 파일 | 내용 |
|---|---|
| `srcs/render/lighting.c` | `apply_specular` + `lighting(..., view_dir)` |
| `srcs/render/render_pixel.c` | `V = normalize(camera.position − P)` 를 lighting에 전달 |
| `includes/render.h` | 시그니처, `SPECULAR_KS` / `SPECULAR_SHININESS` |

외부 경계 `t_color render_pixel(t_scene *, int, int)` 는 그대로다. 카메라 훅·파서·`.rt` 문법은 손대지 않았다.

---

## 3. 데모 씬

`phong.rt`

- 낮은 ambient (`0.12`) — 하이라이트가 묻히지 않게
- 빨간 구 `sp 0,1,0` 지름 2, 회색 바닥 평면
- 카메라 `(0, 1.4, 6.5)` 가 구를 정면 가까이에서 봄
- 광원 `(2.2, 4.8, 3.5)` — 구의 **오른쪽 위**에서 `R ≈ V` 가 되도록

```sh
make
./miniRT playground/01/phong.rt
```

### before / after

| | 보이는 것 |
|---|---|
| before (ambient+diffuse) | 빨간 구, 광원 쪽이 밝고 하이라이트 없음. 바닥은 매트 |
| after (+specular) | 구 오른쪽 위에 흰/분홍 점. 그림자는 그대로 |

`screenshot.png` — Phong 적용 후. `before_after.png` — 왼쪽 before / 오른쪽 after.

그림자 쪽 구/바닥은 그대로 어둡다.

---

## 4. 함정

**`R`을 `2N−L`로 적기.** `2·dot(N,L)·N − L` 이다. `N`과 `L`이 단위벡터여야 `R`도 단위다.

**스펙큘러에 물체색을 곱하기.** 빨간 구의 하이라이트가 새빨개진다. 광원색만 곱한다.

**그림자에서도 스펙큘러를 더하기.** 가려진 면에 점이 생긴다. `k ≤ 0` / `in_shadow` 이면 ambient만.

**`V` 방향.** `camera − P` (표면에서 눈을 향함). 카메라 광선 방향 그대로 넣으면 하이라이트가 뒤집힌다.

---

## 5. 통과 기준

- [ ] `make` 후 `./miniRT playground/01/phong.rt` 에서 구에 하이라이트가 보인다
- [ ] 기존 `scenes/valid/*.rt` 파서가 통과하고, sp/pl/cy/그림자가 깨지지 않는다
- [ ] 광원을 등지거나 그림자인 픽셀은 ambient만
- [ ] `.rt`에 `ks` / `shininess` 필드를 추가하지 않았다
