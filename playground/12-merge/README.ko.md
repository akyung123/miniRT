# 12단계 — 서연 님 코드와 합치기

**브랜치**: `learn/12-merge` (학습 기록 + 검사 도구) · `feature/render-pixel` (제출용 코드만, `origin/main`에서 새로 땀)

학습 폴더(`playground/10-interaction`)의 렌더러를 실제 프로젝트 구조로 옮겼습니다. **서연 님 코드는 한 줄도 고치지 않았고**, 서연 님이 정한 방식에 렌더러를 맞췄습니다.

## 합친 것

| 가져온 것 | 어디서 | 상태 |
|---|---|---|
| 파서·출력·`main.c`·`error.c`·`parsing.h`·`output.h` | `origin/main` | 바이트까지 동일 |
| `vec3_math.c`·`vec3_ops.c`·`miniRT.h` 선언 | `origin/feature/vec3-render-interface` (아직 main에 없음) | 바이트까지 동일. 렌더러는 이 vec3를 그대로 씀 |
| Makefile | `origin/main` | SRCS 끝에 12개 파일 추가만 (기존 마지막 줄에 `\`) |

## 서연 님 코드에 맞춘 것

| 서연 님 방식 | 렌더러 |
|---|---|
| 모듈마다 폴더 하나 (`srcs/parsing`, `srcs/output`) | `srcs/render/` 하위 폴더 없이 10개 파일 |
| 모듈 헤더 (`parsing.h`, `output.h`) | `includes/render.h` — `t_ray`, `t_hit`, `t_range`, `t_cy`, `t_view`는 여기에만 |
| Makefile에 파일 이름 명시 | 같은 방식으로 12줄 |
| vec3는 `feature/vec3-render-interface` | 제 vec3 복사본 삭제, 서연 님 것 사용 (나중에 그 브랜치가 들어와도 중복 정의 없음) |
| 함수 25줄·인자 4개 이내, 함수 안 주석 없음 | 42 헤더 외 주석 0줄, 인자 4개 이내로 바꿈 (`hit_sphere(r, sp, range)` 등) |

```
srcs/render/
  ray.c          ray_at
  camera.c       기저·fov 캐시, camera_ray
  sphere.c       hit_sphere
  plane.c        hit_plane
  cylinder.c     측면·높이·뚜껑, hit_cylinder
  normal.c       sphere_normal, cylinder_normal
  hit_scene.c    도형별 t → t_hit, 가장 가까운 교차
  lighting.c     ambient + diffuse
  shadow.c       그림자 광선 (광원 쪽 끝 T_MIN 여유 포함)
  render_pixel.c 하늘 배경, render_pixel
```

## 검사 — `sh merge_test.sh`

`learn/12-merge`와 `feature/render-pixel` 양쪽에서 **44개 통과, 실패 0** (2026-09-17).

| 단계 | 내용 | 결과 |
|---|---|---|
| 1 | 서연 님 파일이 원본 브랜치와 같은가, Makefile은 추가만 했는가 | 10/10 |
| 2 | 루트 `make re` (`-Wall -Wextra -Werror`) | 성공, 경고 0 |
| 3 | 파서·출력·vec3·렌더러·libft 전역 심볼 중복 | 0 |
| 4 | 42 norm 자가 점검 (`tools/normcheck.py`) | 위반 0, 주석 0줄 |
| 5 | 서연 님 파서 자체 테스트 `make -C srcs/parsing suite` | 15/15 |
| 6 | 학습 단계 테스트 04~09를 srcs 코드로 | 04 9/10 (예전부터 있던 케이스 모순), 05~09 전부 |
| 7 | 렌더 회귀: 11단계 씬 175장을 옮기기 전 코드와 픽셀 비교 | 175/175 동일, 파서 머지 전후도 175/175 동일 |
| 8 | 실제 `./miniRT`: 잘못된 인자·파일 17개는 `Error` + 종료 1, valid 씬 7개는 창 유지·실행 중 누수 0 | 전부 |
| 9 | 아직 main에 없는 원격 브랜치 가상 머지 | `scene-builder-controls` 깨끗. `readme-and-comments`는 **서연 님 브랜치끼리** `miniRT.h` 충돌 (렌더러 무관) |

```sh
cd playground/12-merge
sh merge_test.sh                                   # 이 브랜치
ROOT=<feature/render-pixel 작업 트리> sh merge_test.sh
```

학습 테스트는 코드를 고치지 않고 `tools/shim/miniRT.h`와 `tools/contract_adapter.c`로 예전 함수 모양(인자 5~7개)을 새 모양에 이어 붙여 돌립니다.

## 머지하면서 찾은 것

- **`origin/main`에서 `str_to_double.c`가 분리돼** 파서 파일 이름을 나열하던 제 테스트 러너 두 곳(`tests/run.sh`, `11-test/build.sh`)이 링크에 실패 → 폴더째 링크하도록 고침 (서연 님 코드 문제 아님)
- 로컬 `main`이 `origin/main`보다 9커밋 뒤였음. 학습 브랜치들은 옛 파서로 만들어졌지만, 175장 렌더가 같아서 파싱 결과는 바뀌지 않음
- 서연 님 파일에는 42 헤더가 없음 → norminette를 돌리면 걸림 (수정 안 함, 역할 분담 문서에 기록)

## 아직 못 한 것

- **norminette로 정식 확인** — 이 맥에 없어서 자가 점검만 했습니다
- `feature/render-pixel`은 **아직 push 안 함**
- 10단계 이동(보너스)은 `srcs`에 넣지 않았습니다
