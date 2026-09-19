# miniRT

42 project — a raytracer built with MiniLibX. Reads a `.rt` scene description
file, ray-traces it, and displays the result in a window.

## Authors

| Login | Area |
|---|---|
| [seoykim](https://github.com/seoykim) | Scene parsing (`.rt` format), input, MiniLibX window/render loop |
| [akkim](https://github.com/akyung123) | Shape intersection, color & lighting calculation |

## Build

```bash
make        # build (fetches/builds libft and minilibx as needed)
make clean  # remove object files
make fclean # remove object files and the binary
make re     # fclean + build
```

Builds on both macOS (minilibx_macos_opengl) and Linux (minilibx-linux) —
the Makefile picks the right minilibx variant via `uname`.

## Usage

```bash
./miniRT <scene>.rt
```

- **ESC** or clicking the window's close button exits the program.
- The window size is fixed (`WIN_WIDTH` / `WIN_HEIGHT` in `includes/miniRT.h`).

## Scene file (`.rt`) format

| Identifier | Element | Format |
|---|---|---|
| `A` | Ambient lighting | `A <ratio 0-1> <R,G,B>` |
| `C` | Camera | `C <x,y,z> <dir x,y,z> <FOV 0-180>` |
| `L` | Light | `L <x,y,z> <brightness 0-1> <R,G,B>` |
| `sp` | Sphere | `sp <center x,y,z> <diameter> <R,G,B>` |
| `pl` | Plane | `pl <point x,y,z> <normal x,y,z> <R,G,B>` |
| `cy` | Cylinder | `cy <center x,y,z> <axis x,y,z> <diameter> <height> <R,G,B>` |

Each scene needs exactly one `A`, one `C`, and one `L`; objects (`sp`/`pl`/`cy`)
can appear any number of times.

## Memory check (valgrind)

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	./miniRT scenes/valid/example.rt
```

Only `definitely lost` / `indirectly lost` bytes count as real leaks —
`still reachable` blocks from mlx/X11 itself are expected and not a bug.
To hide that library noise from the report, generate a suppression file
once on the actual grading machine (the mlx symbols differ from macOS to
Linux, so a suppression file must come from Linux to be useful there):

```bash
valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all \
	--gen-suppressions=all ./miniRT scenes/valid/example.rt > /tmp/gen.supp
```

(`--errors-for-leak-kinds=all` is required — by default valgrind only counts
`definite`/`possible` leaks as "errors", and `--gen-suppressions` only emits
a suppression block per error. Without it, `still reachable` leaks — the
mlx/X11 ones we actually want suppressed — produce no output at all.)

From the generated output, keep only the suppression blocks whose stack
trace has **no `srcs/` frames** (mlx/X11-only ones), save them as
`.valgrind.supp`, then run with:

```bash
valgrind --leak-check=full --show-leak-kinds=all --suppressions=.valgrind.supp \
	./miniRT scenes/valid/example.rt
```

## Tools

[`tools/scene_builder.html`](tools/scene_builder.html) — a standalone,
browser-based scene editor. Lets you place objects, move the camera, and
generates the `.rt` text live. Open the file directly in a browser, no
server needed:

```bash
open tools/scene_builder.html   # macOS; xdg-open on Linux
```

**Controls:**

| Action | How |
|---|---|
| Select / deselect an object | Click its name chip in the sidebar (click again to deselect) |
| Move the selected object | Drag on the canvas, or WASD / arrow keys (x/z), Q/E (height) |
| Rotate the selected object | Shift+drag on the canvas — changes `normal` (plane) / `axis` (cylinder); spheres have no orientation, so nothing to rotate |
| Orbit the camera | Drag on the canvas while nothing is selected |
| Move the camera | WASD while nothing is selected (moves along the camera's own forward/right, ignoring pitch) — Q/E for height |

The top-left corner of the canvas always shows what's currently being
controlled (camera or a specific object) and which keys apply.
