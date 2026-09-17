# 9단계 — 실제 씬 연결

**브랜치**: `git checkout -b learn/09-scene learn/08-cylinder`
**기간**: 1~2일

**한 줄 요약**: 하드코딩한 카메라와 도형을 **파서가 준 `t_scene`으로** 갈아끼운다.

---

## 0. 왜 이걸 하는가

지금까지 카메라는 원점에 고정, 도형은 코드에 박아둔 상태였습니다. 이제 `.rt` 파일이 시키는 대로 그려야 합니다.

작업은 크게 둘입니다:

1. **카메라** — 임의 위치에서 임의 방향을 보는 카메라 만들기 (**여기가 어렵습니다**)
2. **데이터 연결** — 도형/조명을 `scene`에서 읽기 (기계적입니다)

그리고 마지막에 함수 시그니처를 팀 규약에 맞춥니다.

---

## 1. 카메라 — 직교기저

### 문제

카메라가 `(-50, 0, 20)`에 있고 `(0,0,1)` 방향을 본다면, 화면의 "오른쪽"과 "위쪽"은 월드 좌표로 어느 방향일까요? 그걸 계산하는 게 **직교기저(orthonormal basis)** 만들기입니다.

### 만드는 법

```
forward = camera.orientation            /* 파서가 이미 정규화해줌 */
right   = normalize(cross(forward, world_up))    /* world_up = (0,1,0) */
up      = cross(right, forward)                  /* 이미 단위벡터 */
```

**순서가 중요합니다.** `cross(a,b) = -cross(b,a)`라서 뒤바꾸면 화면이 좌우/상하로 뒤집힙니다.

### 예외 — 반드시 처리

**`forward`가 `world_up`과 평행하면 `cross`가 영벡터가 됩니다.** 카메라가 정확히 위나 아래를 볼 때입니다 (`C 0,0,0 0,1,0 70`).

```
if (fabs(dot(forward, world_up)) > 1 - 1e-6)
    world_up = (0, 0, 1);    /* 다른 축으로 대체 */
```

이 예외를 빠뜨리면 그 씬에서 화면이 새까맣게 나오거나 NaN이 퍼집니다. **원인 찾기가 매우 어려우니 지금 넣으세요.**

---

## 2. FOV → 뷰포트

`.rt`의 fov는 **도(degree) 단위 수평 화각**입니다.

```
theta = fov * M_PI / 180.0
half_width  = tan(theta / 2)
half_height = half_width / aspect        /* aspect = WIN_WIDTH / WIN_HEIGHT */
```

`tan`이 왜 나오는지는 직각삼각형 하나 그려보면 바로 보입니다. 카메라에서 거리 1 앞에 화면이 있다고 두면, 화면 절반 폭이 `tan(θ/2)`입니다.

### 광선 만들기

```
u = (2 * (x + 0.5) / WIN_WIDTH  - 1) * half_width
v = (1 - 2 * (y + 0.5) / WIN_HEIGHT) * half_height      /* y 뒤집기 */

dir = normalize(forward + u * right + v * up)
ray = { camera.position, dir }
```

3단계에서 한 것과 같고, 축이 월드 축에서 카메라 축으로 바뀐 것뿐입니다.

---

## 3. 데이터 연결 (기계적)

| 파서가 주는 것 | 주의 |
|---|---|
| `scene->camera.orientation` | **이미 정규화됨** — 다시 안 해도 됨 |
| `scene->ambient.ratio / .color` | 색은 이미 0~1 |
| `scene->light.position / .brightness / .color` | 광원은 **하나** (mandatory) |
| `sphere.diameter` | **반지름 아님 → `/2`** |
| `cylinder.diameter` | **`/2`** |
| `cylinder.center` | **중심** (밑면 아님) |
| `plane.normal`, `cylinder.axis` | **이미 정규화됨** |
| `scene->objects` | 연결 리스트, `type`으로 분기 |

색은 파서가 이미 `/255.0` 해서 0~1로 넣어줍니다. **다시 나누지 마세요.** 화면이 새까매집니다.

---

## 4. 시그니처 맞추기

최종적으로 팀 규약에 맞춰야 합니다:

```c
t_color render_pixel(t_scene *scene, int x, int y);
```

- `width`/`height`는 **인자로 안 받습니다.** `WIN_WIDTH` / `WIN_HEIGHT` 매크로 사용 (norm의 인자 4개 제한 때문)
- 반환은 **0.0~1.0 정규화 색**. 정수 변환은 출력 쪽이 합니다
- 카메라 기저는 **픽셀마다 다시 계산하면 낭비**입니다. 48만 번 `cross`를 부르게 되니, 한 번 계산해서 캐시할 방법을 생각해보세요 (static, 또는 scene에 붙이기, 또는 첫 호출에 초기화)

---

## 5. 구현 순서

- [ ] **1. 카메라 기저 함수** — forward/right/up. 평행 예외 포함
- [ ] **2. fov → 뷰포트**
- [ ] **3. 광선 생성** — 하드코딩 씬으로 먼저 검증 (카메라만 옮겨가며)
- [ ] **4. 방향 시각화** — `(dir+1)*0.5`로 찍어서 카메라를 돌렸을 때 색이 자연스럽게 도는지
- [ ] **5. 도형을 `scene->objects`에서 읽기** — 지름 `/2` 주의
- [ ] **6. 조명을 `scene->ambient` / `scene->light`에서 읽기**
- [ ] **7. `render_pixel` 시그니처로 맞추기**
- [ ] **8. 씬 6개 전부 렌더**

---

## 6. 검증할 씬

`scenes/valid/`의 6개를 전부 돌려야 합니다.

| 파일 | 확인할 것 |
|---|---|
| `example.rt` | 평면+구+원기둥. 기본 |
| `multi_objects.rt` | 구2·평면·원기둥2. 가려짐과 그림자 |
| `no_objects.rt` | **도형 0개.** 배경만 나오고 안 죽어야 함 |
| `boundary_values.rt` | **fov 0**, ambient 0, brightness 0. 극단값에 안 죽는지 |
| `crlf_line_endings.rt` | 파서 몫 — 그냥 잘 그려지면 됨 |
| `no_trailing_newline.rt` | 파서 몫 |

**`no_objects.rt`와 `boundary_values.rt`가 크래시 테스트**입니다. fov 0이면 `tan(0)=0`이라 뷰포트 폭이 0 — 0으로 나누는 곳이 없는지 확인하세요.

---

## 7. 함정

**카메라 기저 예외 누락** → 위/아래를 보는 씬이 검게 나옴

**지름을 반지름으로 사용** → 모든 구가 두 배 크게 나옴

**색을 두 번 나눔** → 화면이 거의 검정

**y 뒤집기 누락** → 그림이 상하 반전

**cross 순서 뒤바뀜** → 좌우 반전. 거울에 비친 것처럼 보입니다

**픽셀마다 기저 재계산** → 느립니다. 동작은 하니 나중에 정리해도 됩니다

---

## 테스트 — 최종 인수 테스트

```sh
cd ../tests && sh run.sh 9
```

여기서부터는 놀이가 아니라 **과제가 통과하는가**를 봅니다. mlx 없이 **진짜 파서**(`srcs/parsing`)와 **진짜 씬 파일**로 `render_pixel`을 격자로 돌립니다.

**요구하는 함수** — 이건 팀 규약이라 **바꾸면 안 됩니다**:

```c
t_color	render_pixel(t_scene *scene, int x, int y);
```

**검사 19개**:

| 씬 | 검사 |
|---|---|
| `example.rt` | NaN/inf 없음, 색 ∈ [0,1], **단색 아님**(뭔가 그려짐) |
| `multi_objects.rt` | 〃 (가려짐·그림자 포함) |
| `crlf_line_endings.rt` / `no_trailing_newline.rt` | 〃 |
| **`no_objects.rt`** | 도형 0개에도 안 죽음 |
| **`boundary_values.rt`** | **fov 0**, ambient 0, brightness 0 → 0으로 나누기·NaN 없음 |
| `invalid/*.rt` 9개 | 파서가 전부 거부 |

> 이 하네스는 기존 `tests/render_pixel_stub.c`(그라디언트)로 미리 검증했습니다 — 19/19 통과. 여기서 실패가 뜨면 **하네스가 아니라 내 렌더러 문제**입니다.

**이 테스트가 못 잡는 것**: 그림이 맞게 보이는가, 시각적 얼룩, 성능, norm, 메모리 누수. 각각 눈·norminette·`leaks`로 따로 확인해야 합니다.

## 8. 통과 기준

- [ ] `scenes/valid/*.rt` 6개가 전부 렌더되고 크래시 없음
- [ ] `example.rt`가 의도한 대로 보인다 (평면 위 구, 옆에 원기둥)
- [ ] fov를 바꾸면 화각이 넓어지고 좁아진다
- [ ] 카메라 위치/방향을 바꾸면 그에 맞게 시점이 움직인다
- [ ] `t_color render_pixel(t_scene *, int, int)` 시그니처를 지킨다

말로 답해보기:

- `right = cross(forward, up)`에서 순서를 바꾸면 왜 좌우가 뒤집히나?
- 카메라가 정확히 위를 볼 때 무슨 일이 일어나나? 왜?
- fov에 왜 `tan`이 나오나?

---

## 다음

여기까지가 **mandatory 전부**입니다. 이제 `feature/render-pixel` 브랜치로 정리해서 옮기면 됩니다 (`../ROADMAP.ko.md`의 마무리 체크리스트).

10단계 이동은 보너스이고, 시간이 남을 때만.
