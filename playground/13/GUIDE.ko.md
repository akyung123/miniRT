# 13단계 — main 스냅샷

**브랜치**: `learn/13-main` (`learn/12-merge`에서 분기)

12단계까지 합친 **현재 main 렌더러를 `playground/13`에 그대로 복사**한 폴더입니다. 제출용 코드가 아니라, 보너스 단계를 붙이기 전에 “mandatory가 끝난 상태”를 고정해 둔 것입니다.

루트 `srcs/` · `includes/` · Makefile 은 건드리지 않습니다. 이 폴더만 빌드합니다.

```sh
cd playground/13
make
./miniRT scenes/valid/example.rt
```

`libft` 와 minilibx 는 루트를 가리키는 심볼릭 링크입니다.

## 이 스냅샷에 있는 것

| 경로 | 내용 |
|---|---|
| `includes/` `srcs/` `Makefile` | main과 동일한 파서 + 출력 + `render_pixel` |
| `scenes/valid` · `scenes/invalid` | 기존 씬 |
| `tests/` | 파서/출력 모듈 테스트 소스 |

조명는 ambient + Lambert diffuse 한 점광원입니다. 스펙큘러, 체크무늬, 여러 광원, 원뿔, bump 는 **다음 단계에서 폴더를 따로** 만듭니다.

## 다음

보너스 항목은 **다시 `learn/12-merge`에서 브랜치를 따고** `playground/14-…` 폴더를 새로 만듭니다. 13을 main에 머지하지 않습니다.
