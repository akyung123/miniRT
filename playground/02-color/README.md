# Step 2 — 색

`0~255 정수 색`과 `0.0~1.0 실수 색` 사이를 자유롭게 오간다.

## 왜 여기서 이름을 본체에 맞추나

`includes/miniRT.h` 와 `includes/output.h` 가 정본이다. 이 폴더는 그 이름을
그대로 쓴다 — `WIN_WIDTH`, `t_vec3`, `t_color`, `t_mlx`, `t_minirt`,
`init_mlx`, `render_all`, `color_to_int`, `key_hook`, `KEY_ESC`.

00단계의 `WIDTH` / `t_mini` / `t_point` / `create_argb` / `get_color_val` /
`draw_c2` / `key_event` 는 전부 버렸다. 4단계에서 파서를 붙일 때
`includes/miniRT.h` 를 통째로 갖다 놓기만 하면 되게 하는 것이 목적이다.

**파서 코드는 한 줄도 안 가져왔다.** 2단계에 필요한 건 파서가 지키는 규약
하나뿐이고 (`srcs/parsing/parse_numbers.c` 의 `parse_color` 가 `.rt` 의
`255,0,0` 을 `/255.0` 해서 넣어준다), 그건 코드가 아니라 사실이다.

## 파일

| 파일 | 본체 대응 | 상태 |
|---|---|---|
| `includes/miniRT.h` | `includes/miniRT.h` 의 부분집합 | 완성 |
| `includes/output.h` | `includes/output.h` | 완성 |
| `main.c` | `srcs/main.c` | 완성 |
| `mlx_init.c` | `srcs/output/mlx_init.c` | 완성 |
| `hooks.c` | `srcs/output/hooks.c` | 완성 |
| `render_loop.c` | `srcs/output/render_loop.c` | 완성 |
| `color.c` | (본체는 `render_loop.c` 안에 있음) | **비어 있음 — 여기가 과제** |
| `tests/color_test.c` | 없음 | 완성 (검사 14개) |

## 할 일

`color.c` 의 세 함수를 채운다. 그게 전부다.

```c
static double	clamp(double v, double lo, double hi);
int				color_to_int(t_color color);
t_color			color_lerp(t_color a, t_color b, double t);
```

```sh
make test     # 14개 검사. 지금은 9개 떨어진다
make && ./color
```

`color_to_int` 를 채우는 순간 창에 2축 그라디언트가 뜬다 — 지금은 0만
돌려주므로 화면이 새까맣다. 그게 파이프라인이 살아 있다는 증거다.

그 다음 `render_loop.c` 의 `render_pixel` 본문을 주석에 적힌 순서대로
갈아끼운다 (단색 -> 좌우 -> 2축 -> lerp -> 클램프 시험). 나머지 파일은
2단계 내내 안 건드린다.

## 통과 기준

- `color_to_int` 가 범위 밖 입력에도 안전하다 (`make test` 14/14)
- 위아래 lerp 그라디언트에 띠가 안 보인다 (보이면 정수 나눗셈을 의심)
- `srcs/output/render_loop.c:12` 의 `color_to_int` 와 내 것을 비교해서
  **왜 같은지** 설명할 수 있다. `playground/00-mlx/draw.c` 의
  `get_color_val`(`clamp(0, 0.999) * 256`)과도 비교할 것

말로 답해보기:

- 왜 렌더러 내부에서는 0~1 실수로 다루나?
- 빨간 물체에 파란 빛을 비추면 왜 검정인가?
- 클램프를 안 하면 정확히 무슨 일이 일어나나?
- `WIN_WIDTH` 가 아니라 `WIN_WIDTH - 1` 로 나누는 이유는?

## 다음

3단계 — 광선. 여기서 만든 `color_lerp` 가 그대로 하늘 배경이 된다.
파서는 여전히 필요 없다.
