# playground/13 — frozen main snapshot

이 폴더는 **보너스 학습용으로 고정한 main 스냅샷**입니다.  
This folder is a **frozen copy of `main`** for bonus-part study.

스냅샷 기준 / snapshotted from: `b9c9eab` (`fix(main): 카메라 이동을 빼고 예시 씬을 도형 쪽으로 맞춤`).

**이 PR에서는 보너스를 구현하지 않습니다.** 여기 있는 코드는 mandatory 기준선입니다.  
**No bonuses in this PR** — sources here are the mandatory baseline only.

## 예정된 학습 폴더 / Planned sibling folders

아직 만들지 않았습니다. 나중에 이 스냅샷 옆(또는 아래)에 둡니다.  
Not created yet. These will be added later as siblings (or subfolders) of this snapshot.

| Folder | Topic |
|---|---|
| `01` | Phong specular |
| `02` | Checkerboard |
| `03` | Colored multi lights |
| `04` | Cone (quadric) |
| `05` | Bump map |

## Layout

루트 프로젝트와 같은 배치입니다. 바이너리(`.o` / `.a` / `miniRT` 등)는 복사하지 않았습니다.

```
playground/13/
  Makefile
  includes/
  srcs/
  libft/
  scenes/
  tests/
  minilibx_macos_opengl -> ../../minilibx_macos_opengl   (symlink, not a copy)
  README.md
```

MiniLibX 소스는 용량·플랫폼 이슈를 피하려고 복사하지 않고, 리포 루트의 `minilibx_macos_opengl`을 가리킵니다.

## Build

이 폴더가 작업 디렉터리입니다. 루트 `make`는 이 스냅샷을 쓰지 않습니다.

```sh
cd playground/13
make
./miniRT scenes/valid/example.rt
```

Linux에서는 루트에 `minilibx-linux`가 있을 때 같은 방식으로 링크하면 됩니다.

```sh
ln -s ../../minilibx-linux minilibx-linux
make
```

Parser suite (mlx 불필요):

```sh
cd playground/13/srcs/parsing && make suite
```
