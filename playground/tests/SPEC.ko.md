# 테스트 명세

**합격 기준은 42 과제와 같습니다: 그 단계에 주어진 파일이 전부 통과하면 그 단계 완료.**

값이 C 코드에 박혀 있지 않습니다. 각 단계는 `cases/<단계>/` 안의 **케이스 파일**로 평가하고, 9단계만 예외로 **실제 `scenes/*.rt`** 를 그대로 씁니다. 지금 `scenes/valid`, `scenes/invalid`가 파서에 대해 하는 역할을 모든 단계로 넓힌 구조입니다.

```sh
sh run.sh        # 1~9단계 전부
sh run.sh 4      # 4단계만
```

아직 구현 안 한 단계는 링크가 안 되므로 **SKIP**으로 넘어갑니다. ASAN이 켜져 있고, 실패가 하나라도 있으면 종료 코드 1.

---

## 케이스 파일 문법

`.rt`를 닮게 만들었습니다. 식별자로 시작하는 줄, 콤마로 묶은 벡터, `#` 주석.

```
# 주석
<명령> <인자> <인자> ... => <기대값> [~허용오차]
```

- 벡터는 `x,y,z`
- **도형은 `.rt`와 똑같이 '지름'으로 적습니다.** 러너가 `/2`해서 반지름으로 넘깁니다
- 기대값 키워드: `MISS`(안 맞음) `HIT`(맞기만 하면 됨) `EMPTY`(안 칠해짐) `SHADOW`/`LIT` `POS`/`NEG` `YES` `WIDER`
- `~0.01` 처럼 허용오차를 붙일 수 있습니다 (기본 `1e-6`)

**케이스를 추가하려면 텍스트 파일에 한 줄 쓰면 됩니다.** 재컴파일 필요 없습니다. 직접 만든 케이스를 넣어보는 게 이 구조의 요점입니다.

---

## 명령 목록

| 단계 | 디렉터리 | 명령 |
|---|---|---|
| 1 | `cases/01-pixels/` | `img <pad>` `put <x,y> <색>` `circle <x,y> <r> <색>` `square <x1,x2> <y1,y2> <색>` `px <x,y> =>` `count =>` `argb <a> <r> <g> <b> =>` |
| 2 | `cases/02-color/` | `to_int <r,g,b> =>` `alpha <r,g,b> =>` `lerp <a> <b> <t> =>` |
| 3 | `cases/03-ray/` | `add` `sub` `scale` `dot` `cross` `length` `normalize` `norm_len` `ray_at` `cam_len` `cam_dir` `cam_sx` `cam_sy` `cam_symmetry` `cam_aspect` |
| 4 | `cases/04-sphere/` | `hit <시작> <방향> <중심> <지름> <tmin> <tmax> =>` |
| 5 | `cases/05-shading/` | `normal <중심> <지름> <점> =>` `normal_len` `diffuse <n> <l> =>` `ambient <물체> <환경> <비율> =>` |
| 6 | `cases/06-shadow/` | `sp <중심> <지름> <rgb>` (등록) `shadow <점> <광원> =>` (평가 후 초기화) |
| 7 | `cases/07-plane/` | `hit <시작> <방향> <점> <법선> <tmin> <tmax> =>` |
| 8 | `cases/08-cylinder/` | `hit <시작> <방향> <중심> <축> <지름> <높이> <tmin> <tmax> =>` |
| 9 | `../../scenes/` | 명령 없음. 실제 `.rt` 파일을 파서로 읽어 렌더 |

---

## 함수 계약

러너가 부르는 함수입니다. **이름을 바꾸고 싶으면 `test_0N.c` 상단의 선언 한 줄만 고치면 됩니다** — 케이스 파일은 그대로 씁니다.

| 단계 | 시그니처 |
|---|---|
| 1 | `void my_mlx_pixel_put(t_mlx*, int x, int y, int color)` |
| | `void draw_circle(t_mlx*, int cx, int cy, int r, int color)` |
| | `void draw_square(t_mlx*, int x1, int x2, int y1, int y2, int color)` |
| | `int create_argb(int a, int r, int g, int b)` |
| 2 | `int color_to_int(t_color)` / `t_color color_lerp(t_color a, t_color b, double t)` |
| 3 | `vec3_add/sub/scale/cross/dot/length/normalize`, `t_vec3 ray_at(t_ray, double)`, `t_ray camera_ray(int x, int y)` |
| 4 | `double hit_sphere(t_ray, t_vec3 c, double radius, double tmin, double tmax)` |
| 5 | `t_vec3 sphere_normal(t_vec3 c, double radius, t_vec3 p)` |
| | `double diffuse_factor(t_vec3 n, t_vec3 light_dir)` |
| | `t_color apply_ambient(t_color obj, t_color amb, double ratio)` |
| 6 | `int shadow_test(t_vec3 p, t_vec3 light, t_sphere_lite *b, int n)` |
| 7 | `double hit_plane(t_ray, t_vec3 p0, t_vec3 n, double tmin, double tmax)` |
| 8 | `double hit_cylinder(t_ray, t_vec3 c, t_vec3 axis, double radius, double height, double tmin, double tmax)` |
| 9 | `t_color render_pixel(t_scene*, int x, int y)` ← **팀 규약. 고정입니다** |

타입 가정: `t_vec3 { double x, y, z; }`, `t_color = t_vec3`, `t_ray { t_vec3 origin, direction; }`, `t_sphere_lite { t_vec3 center; double radius; }`.

교차 함수는 **맞으면 `t`, 안 맞으면 음수**로 통일했습니다.

---

## 단계별 케이스 파일

### 1단계 `cases/01-pixels/` — 7개 파일, 28개 검사 ✅ 통과함

| 파일 | 무엇을 보는가 |
|---|---|
| `01_put_pixel.case` | 주소 계산, 옆/아랫줄 안 건드리는가 |
| **`02_size_line.case`** | **패딩 64바이트. `width*4` 하드코딩이면 실패** |
| `03_bounds.case` | 화면 밖 좌표 5개가 버퍼를 안 건드리는가 |
| `04_circle.case` | 원이 속이 차는가, 경계가 정확한가 |
| `05_circle_clipped.case` | 가장자리에 걸친 원에서 버퍼가 새는가 |
| `06_square.case` | 반열린 구간, 정확히 1200픽셀 |
| `07_color.case` | 최상위 바이트가 0인가 (255면 완전 투명) |

### 2단계 `cases/02-color/` — 3개 파일

`01_pack.case` 채널 위치와 중간값 · **`02_clamp.case` 범위 밖 입력** · `03_lerp.case` 보간

### 3단계 `cases/03-ray/` — 5개 파일

`01_vec3.case` 기본 연산 · **`02_cross.case` 순서와 평행(9단계 예외의 원인)** · `03_normalize.case` · `04_ray.case` `P(t)=A+tb` · **`05_camera.case` y 뒤집기와 종횡비**

### 4단계 `cases/04-sphere/` — 3개 파일

`01_basic.case` 정면·빗나감·접함·지름 · **`02_roots.case` 카메라 뒤, 구 내부(두 근)** · `03_range.case` tmin/tmax

### 5단계 `cases/05-shading/` — 3개 파일

`01_normal.case` · `02_diffuse.case` 등진 면 클램프 · **`03_ambient.case` 빨강×파란빛=검정**

### 6단계 `cases/06-shadow/` — 3개 파일

`01_basic.case` · **`02_distance.case` 광원보다 먼 물체** · **`03_acne.case` 자기 자신**

### 7단계 `cases/07-plane/` — 2개 파일

`01_basic.case` (subject 예시 `pl 0,0,-10 0,1,0` 포함, 점을 평면 안에서 옮겨도 같은 결과) · `02_edge.case` 평행·등짐·45도

### 8단계 `cases/08-cylinder/` — 4개 파일

구현 순서 (a)→(b)→(c)에 맞춰 나눠뒀습니다. **하나씩 켜지는 걸 보면서 진행하세요.**

`01_side.case` 측면 · **`02_height.case` 높이 자르기 + 중심/밑면 확인** · **`03_cap.case` 뚜껑** · `04_axis.case` 임의 축, 두 근

### 9단계 `../../scenes/` — 실제 씬

케이스 파일이 아니라 **진짜 `.rt` 파일**입니다. mlx 없이 진짜 파서로 읽어 격자 렌더합니다.

| 씬 | 검사 |
|---|---|
| `valid/example.rt` `multi_objects.rt` `crlf_line_endings.rt` `no_trailing_newline.rt` | NaN/inf 없음, 색 ∈ [0,1], 단색 아님 |
| **`valid/no_objects.rt`** | 도형 0개에도 안 죽음 |
| **`valid/boundary_values.rt`** | fov 0, ambient 0, brightness 0 |
| `invalid/*.rt` 9개 | 파서가 전부 거부 |

> 이 하네스는 `tests/render_pixel_stub.c`(그라디언트)로 미리 검증했습니다 — 19/19 통과. 여기서 실패가 뜨면 **하네스가 아니라 내 렌더러 문제**입니다.

---

## 테스트가 잡지 못하는 것

정직하게 적어둡니다. 아래는 **눈으로** 봐야 합니다.

- 그림이 "맞게" 보이는가 (구도, 원근, 색감)
- shadow acne 같은 시각적 얼룩 — 6단계 케이스가 논리는 잡지만 화면 얼룩은 못 봅니다
- 성능
- norm 통과 (norminette 별도)
- 메모리 누수 (`leaks -atExit -- ./miniRT scenes/valid/example.rt` 별도)

**테스트 통과 = 과제 통과가 아닙니다.** 계산이 맞다는 것까지만 보장합니다.
