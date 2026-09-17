NAME = miniRT
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = srcs/main.c \
	srcs/error.c \
	srcs/parsing/free_scene.c \
	srcs/parsing/parse_elements.c \
	srcs/parsing/parse_numbers.c \
	srcs/parsing/parse_objects.c \
	srcs/parsing/parse_scene.c \
	srcs/parsing/parse_utils.c \
	srcs/parsing/str_to_double.c \
	srcs/output/cleanup.c \
	srcs/output/hooks.c \
	srcs/output/mlx_init.c \
	srcs/output/render_loop.c \
	srcs/vec3_math.c \
	srcs/vec3_ops.c \
	srcs/render/ray.c \
	srcs/render/camera.c \
	srcs/render/sphere.c \
	srcs/render/plane.c \
	srcs/render/cylinder.c \
	srcs/render/normal.c \
	srcs/render/hit_scene.c \
	srcs/render/lighting.c \
	srcs/render/shadow.c \
	srcs/render/bump.c \
	srcs/render/render_pixel.c
OBJS = $(SRCS:.c=.o)

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
	MLX_DIR = ./minilibx_macos_opengl
	MLX_LIBS = -lmlx -framework OpenGL -framework AppKit
else
	MLX_DIR = ./minilibx-linux
	MLX_LIBS = -lmlx -lXext -lX11
endif

MLX = $(MLX_DIR)/libmlx.a
INCLUDE = -I./includes -I$(MLX_DIR) -I./libft

LIBS = -L$(LIBFT_DIR) -L$(MLX_DIR) -lft $(MLX_LIBS) -lm

.PHONY : all
all: $(LIBFT) $(MLX) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(MLX):
	make -C $(MLX_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(LIBS) -o $(NAME)

.PHONY: clean
clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean
	make -C $(MLX_DIR) clean

.PHONY: fclean
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

.PHONY: re
re: fclean all

DUMP_SRCS = srcs/error.c \
	srcs/parsing/free_scene.c \
	srcs/parsing/parse_elements.c \
	srcs/parsing/parse_numbers.c \
	srcs/parsing/parse_objects.c \
	srcs/parsing/parse_scene.c \
	srcs/parsing/parse_utils.c \
	srcs/parsing/str_to_double.c \
	srcs/vec3_math.c \
	srcs/vec3_ops.c \
	srcs/render/ray.c \
	srcs/render/camera.c \
	srcs/render/sphere.c \
	srcs/render/plane.c \
	srcs/render/cylinder.c \
	srcs/render/normal.c \
	srcs/render/hit_scene.c \
	srcs/render/lighting.c \
	srcs/render/shadow.c \
	srcs/render/bump.c \
	srcs/render/render_pixel.c \
	dump_ppm.c

.PHONY: dump_ppm
dump_ppm: $(LIBFT)
	$(CC) $(CFLAGS) -I./includes -I./libft $(DUMP_SRCS) -L$(LIBFT_DIR) -lft -lm -o dump_ppm
