# 보너스 02 — Checkerboard (color disruption)

**브랜치**: `cursor/bonus-02-checkerboard-7cbf` (`main`에서 분기)
**한 줄 요약**: 조명 전에 알베도를 격자 두 색으로 바꿔, 바닥이 체스판처럼 보이게 한다.

이 폴더는 **문서 + 데모 씬**만 둔다. 렌더러 코드는 루트 `srcs/render/` 가 본체다. `playground/13` 스냅샷과는 별개다. 보너스 01 Phong 스펙큘러는 다른 PR(#8) — 이 브랜치는 ambient+diffuse+shadow 만 가정한다.

---

## 0. 체커가 더하는 것

Mandatory 최종색은 물체색 `rec->color` 를 그대로 쓴다.

```
color = ambient(albedo) + diffuse(albedo)
```

단색 평면은 그냥 넓은 색 덩어리라 **스케일감이 없다.** 체커보드는 같은 조명에 알베도만 두 색으로 갈아 끼운다.

```
albedo = checker(P) ? color2 : color1
color  = ambient(albedo) + diffuse(albedo)
```

그림자·뒷면은 예전과 같이 **ambient만** (조명 코드 미변경).

---

## 1. 수식

교차점 `P`. `scale > 0` 은 한 칸의 세계 좌표 한 변.

### 평면 — 접선 UV

법선 `n` 에 수직인 단위 축 `t`, `b` 를 만들고:

```
local = P − plane.point
u = dot(local, t)
v = dot(local, b)
odd = (floor(u / scale) + floor(v / scale)) & 1
```

`n ≈ (0,1,0)` 바닥이면 `t=(0,0,1)`, `b=(1,0,0)` → `u=z`, `v=x`. 곧

```
odd = (floor(x / scale) + floor(z / scale)) & 1
```

음수 좌표는 `floor` 를 써야 한다. `(int)(x/scale)` 는 0 쪽으로 잘라서 `x<0` 칸이 두 배로 커진다.

### 구 — 구면 UV (nice-to-have)

```
d = normalize(P − center)
u = atan2(d.z, d.x) · radius     /* 적도 방향 호 길이 */
v = acos(clamp(d.y, −1, 1)) · radius
odd = (floor(u / scale) + floor(v / scale)) & 1
```

`scale` 의미는 평면과 같다 (세계 단위). 극에서는 칸이 모인다.

### 원기둥

축에 상관없이 3D 격자:

```
odd = (floor(x/s) + floor(y/s) + floor(z/s)) & 1
```

### 색

```
albedo = odd ? checker_color : object_color
```

조명 전에 `rec->color` 를 이 값으로 **교체**한다. 이후 ambient/diffuse 는 예전 공식 그대로.

---

## 2. `.rt` 에서 켜기

**선택 키워드 `ck`.** 새 identifier 를 만들지 않았고, mandatory 필드 개수도 그대로다.

| 도형 | mandatory | + checker |
|---|---|---|
| `sp` | 4 토큰 | `ck <scale> <R,G,B>` → 7 |
| `pl` | 4 토큰 | 같음 → 7 |
| `cy` | 6 토큰 | `ck <scale> <R,G,B>` → 9 |

예:

```
pl 0,0,0 0,1,0 232,220,198 ck 1.5 28,26,32
```

- `ck` 없음 → 단색. `scenes/valid` 전부 그대로 통과.
- `ck` 가 아닌 여분 토큰 / `scale <= 0` / 색 범위 오류 → 파싱 실패 (`Error`).
- 맨 앞 identifier 가 `A`/`C`/`L`/`sp`/`pl`/`cy` 가 아니면 예전처럼 실패.

`scale` 과 두 번째 색은 물체마다 다를 수 있다.

---

## 3. 바뀐 파일

| 파일 | 내용 |
|---|---|
| `srcs/render/checkerboard.c` | UV / 3D 격자, `apply_checker` |
| `srcs/render/hit_scene.c` | 교차 후 `obj->checker` 이면 알베도 교체 |
| `includes/miniRT.h` | `t_object` 에 checker 필드 |
| `includes/render.h` | `apply_checker` 선언 |
| `srcs/parsing/parse_objects.c` | 선택 `ck` 토큰 |
| `Makefile` | `checkerboard.c` 추가 |

`lighting.c` / `render_pixel.c` 시그니처는 **안 건드렸다** (Phong PR 과 겹치지 않게). 외부 경계 `render_pixel(t_scene *, int, int)` 도 그대로.

---

## 4. 데모 씬

`checkerboard.rt`

- ambient `0.18` — 칸이 너무 밝게 날아가지 않게
- 바닥 평면 `y=0`, `scale=1.5`, 크림 vs 차콜
- 빨간 체크 구 `sp -2.0,1.25,-0.4` 지름 2.5, `scale=0.55`
- 파란 단색 구 `sp 2.3,0.95,0.8` — 체커가 없는 물체도 그대로인지 대조
- 카메라 `(0, 2.8, 9)` 가 원점 쪽을 내려다봄
- 광원 `(-4, 6.5, 5)` — 왼쪽 위에서 그림자가 바닥에 떨어지게

```sh
make
./miniRT playground/02/checkerboard.rt
```

바닥은 체스판, 왼쪽 구는 체크 무늬, 오른쪽 구는 단색, 구 그림자가 칸 위를 가로지른다.

`screenshot.png` — 헤드리스 `render_pixel` PPM → PNG.

---

## 5. 함정

**`(int)(x/scale)` 로 칸을 나누기.** 음수에서 칸 크기가 깨진다. `floor` 필수.

**조명 후에 곱하기.** 이미 어두운 픽셀에 격자를 씌우면 그림자가 이중으로 먹는다. 알베도만 바꾸고 조명은 그대로.

**mandatory 씬의 토큰 수를 느슨하게 풀기.** `ck` 만 허용해야 `pl ... 255,0,0 garbage` 가 여전히 실패한다.

**구 UV 의 `atan2` 이음.** `u = ±π·r` 에서 한 줄 시임이 생긴다. 데모는 옆에서 봐서 덜 보인다.

**Phong 과 같은 파일.** `lighting()` 시그니처를 바꾸면 #8 과 충돌한다. 체커는 `hit_scene` 에서 `rec->color` 만 바꾼다.

---

## 6. 통과 기준

- [ ] `make` 후 `./miniRT playground/02/checkerboard.rt` 에서 바닥에 격자가 보인다
- [ ] `ck` 없는 `scenes/valid/*.rt` 파서가 통과하고, 그림자가 깨지지 않는다
- [ ] 보너스 01/03/04/05 와 `playground/13` 은 이 PR 에 없음
- [ ] `-Wall -Wextra -Werror`
