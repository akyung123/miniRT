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

## Tools

[`tools/scene_builder.html`](tools/scene_builder.html) — a standalone,
browser-based scene editor. Lets you place objects, move the camera, and
generates the `.rt` text live. Open the file directly in a browser, no
server needed. See the in-app hints for mouse/keyboard controls (drag to
orbit the camera, WASD to move it, click an object to select and drag/rotate
it).
