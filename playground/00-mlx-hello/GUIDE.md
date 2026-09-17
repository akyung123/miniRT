# Step 0 — guided walkthrough

Companion to `README.md`. Everything below is **library API you call**, not
code you write. The parts you write yourself are left as `TODO` on purpose.

Signatures are copied from `minilibx_macos_opengl/mlx.h` — that header is the
only real documentation, read it once end to end.

---

## 1. The five calls you need

### `mlx_init`

```c
void *mlx_init(void);
```

Opens the connection to the graphics system. First thing, always.
Returns `NULL` on failure. Nothing else works before this.

```c
void *mlx;

mlx = mlx_init();
if (!mlx)
    return (1);
```

Note the header declares it `mlx_init()` with an empty parameter list, not
`(void)`. Harmless here, but that is why `-Wstrict-prototypes` would complain.

---

### `mlx_new_window`

```c
void *mlx_new_window(void *mlx_ptr, int size_x, int size_y, char *title);
```

Returns `NULL` on failure. The title is `char *`, not `const char *`, so a
string literal produces a warning under `-Wwrite-strings` (not enabled here).

```c
void *win;

win = mlx_new_window(mlx, 800, 600, "hello");
if (!win)
    return (1);
```

**On macOS the window does not appear until `mlx_loop` runs.** If your program
prints nothing and shows nothing, that is expected until step 5.

---

### `mlx_pixel_put`

```c
int mlx_pixel_put(void *mlx_ptr, void *win_ptr, int x, int y, int color);
```

Origin is the **top-left** corner, and **y grows downward**. Colour is
`0x00RRGGBB` packed into one int.

```c
mlx_pixel_put(mlx, win, 400, 300, 0xFFFFFF);   /* white   */
mlx_pixel_put(mlx, win, 100, 100, 0xFF0000);   /* red     */
mlx_pixel_put(mlx, win, 100, 200, 0x00FF00);   /* green   */
mlx_pixel_put(mlx, win, 100, 300, 0x0000FF);   /* blue    */
```

To build a colour from components:

```c
int color;

color = (r << 16) | (g << 8) | b;   /* r, g, b each 0..255 */
```

There is no bounds check. Writing outside the window is silently ignored on
macOS, not a crash — do not rely on that.

---

### `mlx_key_hook`

```c
int mlx_key_hook(void *win_ptr, int (*funct_ptr)(), void *param);
```

Registers a callback. mlx calls it as:

```c
int  your_key_hook(int keycode, void *param);
```

The `param` you pass at registration comes back as the second argument — that
is how you get your own data inside the callback without globals.

```c
/* registration — this line is library usage */
mlx_key_hook(win, key_hook, &data);
```

```c
/* the callback itself — YOU write this body */
int key_hook(int keycode, void *param)
{
    /* TODO: if keycode is ESC -> exit(0) */
    (void)param;
    return (0);
}
```

ESC keycode is **53 on macOS**, 65307 on Linux. `includes/output.h` already has
the `#ifdef __APPLE__` for this — reuse the macro rather than hardcoding 53.

To find any other keycode, print `keycode` from inside the hook and mash keys.

---

### `mlx_loop`

```c
int mlx_loop(void *mlx_ptr);
```

Infinite event loop. **Never returns.** Any code after it is dead. This is why
you exit from inside a hook (`exit()`), not by falling out of `main`.

```c
mlx_loop(mlx);
return (0);   /* unreachable */
```

---

## 2. Two more you will meet immediately

### `mlx_hook` — the window close button

```c
int mlx_hook(void *win_ptr, int x_event, int x_mask, int (*funct)(), void *param);
```

Event **17** is `DestroyNotify`, the red close button. ESC and the close button
are different events; handling only ESC means the button does nothing.

```c
mlx_hook(win, 17, 0, close_hook, &data);
```

```c
/* YOU write this body */
int close_hook(void *param)
{
    (void)param;
    /* TODO: exit cleanly */
    return (0);
}
```

### `mlx_expose_hook` — why your dot vanishes

```c
int mlx_expose_hook(void *win_ptr, int (*funct_ptr)(), void *param);
```

Called when the window needs repainting (uncovered, resized, refocused).
mlx does **not** remember what you drew — `mlx_pixel_put` paints once and the
pixel is gone after the next expose. Drag another window over yours and watch.

Redrawing on expose is the fix, and that is exactly what
`srcs/output/hooks.c:expose_hook` does with a cached image. You do not need to
solve it here — just see it happen and understand why step 1 exists.

---

## 3. Main skeleton

Structure only. The `TODO`s are your work.

```c
#include <stdlib.h>
#include "mlx.h"

#define WIDTH  800
#define HEIGHT 600

#ifdef __APPLE__
# define KEY_ESC 53
#else
# define KEY_ESC 65307
#endif

typedef struct s_app
{
    void *mlx;
    void *win;
}   t_app;

int key_hook(int keycode, void *param)
{
    /* TODO */
    (void)keycode;
    (void)param;
    return (0);
}

int close_hook(void *param)
{
    /* TODO */
    (void)param;
    return (0);
}

static void draw(t_app *app)
{
    /* TODO: one white dot at the centre.
     *       then a few more until the axes are obvious. */
    (void)app;
}

int main(void)
{
    t_app app;

    app.mlx = mlx_init();
    if (!app.mlx)
        return (1);
    app.win = mlx_new_window(app.mlx, WIDTH, HEIGHT, "hello");
    if (!app.win)
        return (1);
    draw(&app);
    mlx_key_hook(app.win, key_hook, &app);
    mlx_hook(app.win, 17, 0, close_hook, &app);
    mlx_loop(app.mlx);
    return (0);
}
```

Compare against `srcs/main.c` and `srcs/output/mlx_init.c` once yours runs —
same shape, with parsing and an image buffer added.

---

## 4. Build

From `playground/00-mlx-hello/`:

```
cc -Wall -Wextra -Werror -I../../minilibx_macos_opengl \
   main.c \
   -L../../minilibx_macos_opengl -lmlx -framework OpenGL -framework AppKit \
   -o hello
```

If `libmlx.a` is missing: `make -C ../../minilibx_macos_opengl`.
No libft, no `-lm` needed at this step.

---

## 5. Exercises, in order

1. **White dot.** Centre of the window. Stop when you see it.
2. **Axes.** Draw 100 red pixels along x from (0,0), 100 blue along y.
   Confirm with your own eyes that y goes *down*.
3. **Keycodes.** Print `keycode` in the hook, press ESC, arrows, letters.
   Write down the ones you will want later.
4. **ESC + close button.** Both must quit. Verify `exit()` inside a hook is
   the only way out.
5. **Expose.** Cover the window with another app, uncover it. The dot is gone.
   Understand why before moving on — that is the whole motivation for step 1.

Answer these out loud before leaving step 0:

- Why does `mlx_loop` never return?
- Why does `param` exist on every hook?
- Why is `mlx_pixel_put` a bad idea 480,000 times per frame?
