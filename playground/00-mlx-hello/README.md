# Step 0 — mlx hello

Throwaway learning branch. **Delete when done** — nothing here ships.

Goal: prove MiniLibX builds and runs on this machine, and type out a window
from scratch instead of reading someone else's.

## Build target

Everything lives in `playground/` on purpose: the root Makefile globs
`srcs/*.c srcs/parsing/*.c srcs/output/*.c`, so nothing here is picked up and
the real build stays intact. Same trick as `tests/render_pixel_stub.c`.

macOS link flags (see root Makefile):

    -L../../minilibx_macos_opengl -lmlx -framework OpenGL -framework AppKit

## Do

1. Sanity check the environment first — the existing gradient stub should
   already open a window:

       cd srcs/output && make test

   If that fails, it is the toolchain, not your code. Fix it before writing
   anything.

2. Now write it yourself, from a blank file:
   - `mlx_init()` -> connection pointer
   - `mlx_new_window(mlx, 800, 600, "hello")`
   - `mlx_pixel_put(mlx, win, 400, 300, 0xFFFFFF)` — one white dot, centre
   - `mlx_key_hook(win, ..., ...)` — ESC quits (keycode **53** on macOS,
     65307 on Linux; `includes/output.h` has the ifdef)
   - `mlx_loop(mlx)`

3. Draw a few more dots by hand until the coordinate system is obvious.

## Done when

- Window opens, dot is visible, ESC closes it, no crash on close.
- You can say why `mlx_loop` never returns, and why a pixel drawn *before*
  the loop can still disappear (expose events — the window is not a canvas
  that remembers).

## Gotchas

- Screen y grows **downward**. Not maths convention.
- Colour is packed as `0xRRGGBB` in a single int.
- `mlx_pixel_put` talks to the server per call. That is fine for one dot and
  hopeless for 480,000 — that is Step 1's whole point, so do not optimise here.
- The window close button (event 17) is not the same thing as ESC. Only ESC
  is needed at this step.

## Next

`learn/01-2d-pixels` — image buffer, `mlx_get_data_addr`, rectangles/circles.
