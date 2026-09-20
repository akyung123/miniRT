*This project has been created as part of the 42 curriculum by seoykim, akkim.*

# miniRT

## Description

miniRT is a raytracer built with MiniLibX. It reads a scene description from
a `.rt` file — camera, ambient/point lighting, and a handful of geometric
primitives (sphere, plane, cylinder) — computes the color of every pixel by
casting rays from the camera through the scene, and displays the resulting
image in a window.

Work was split between the two authors: `seoykim` owns scene parsing (the
`.rt` format), input handling, and the MiniLibX window/render loop; `akkim`
owns ray-object intersection, surface normals, and color/lighting
calculations.

## Instructions

### Build

```bash
make        # build (fetches/builds libft and minilibx as needed)
make clean  # remove object files
make fclean # remove object files and the binary
make re     # fclean + build
```

Builds on both macOS (minilibx_macos_opengl) and Linux (minilibx-linux) —
the Makefile picks the right minilibx variant via `uname`.

### Run

```bash
./miniRT <scene>.rt
```

- **ESC** or clicking the window's close button exits the program.
- The window size is fixed (`WIN_WIDTH` / `WIN_HEIGHT` in `includes/miniRT.h`).

### Memory check (valgrind)

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	./miniRT scenes/valid/example.rt
```

Only `definitely lost` / `indirectly lost` bytes count as real leaks —
`still reachable` blocks from mlx/X11 itself are expected and not a bug.
`.valgrind.supp` (committed in the repo root) already hides that library
noise — generated on Linux, keeping only suppression blocks whose stack
trace has no `srcs/` frames (mlx/X11-only ones). Run with:

```bash
valgrind --leak-check=full --show-leak-kinds=all --suppressions=.valgrind.supp \
	./miniRT scenes/valid/example.rt
```

## Resources

- [MiniLibX documentation (42 Paris)](https://github.com/42Paris/minilibx-linux)
- Peter Shirley, *Ray Tracing in One Weekend* — reference for the camera
  ray-generation and ray/sphere-plane-cylinder intersection math.
- The `.rt` scene format and mandatory-part requirements come from the
  project subject itself.

**AI usage**: Claude (Claude Code) was used as a pair-programming assistant
throughout `seoykim`'s parts of the project — scene parsing, the
input/MiniLibX output module, and a standalone `.rt` scene-authoring web
tool. Concretely, it helped with: drafting and iterating on parser/output
source files, reviewing naming and error-handling choices against the 42
Norm and the subject's external-function whitelist, debugging build and
memory-check (valgrind) issues, and writing/checking test scene files. All
generated code was read, tested, and understood by the author before being
committed; no code was accepted without review.

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
