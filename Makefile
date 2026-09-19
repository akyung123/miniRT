NAME = miniRT
NAME_BONUS = miniRT_bonus
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# ---------- mandatory ----------
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
	srcs/render/render_pixel.c

# ---------- bonus ----------
# 스펙큘러 · 체크무늬 · 다중 광원 · 원뿔 · 범프.
# 보너스는 t_scene/t_object/t_hit 레이아웃까지 바꾸기 때문에 공용 파일도
# 같이 _bonus 사본으로 둔다 (mandatory 빌드와 섞이면 안 됨).
BONUS_SRCS = srcs/main_bonus.c \
	srcs/error_bonus.c \
	srcs/parsing/free_scene_bonus.c \
	srcs/parsing/parse_elements_bonus.c \
	srcs/parsing/parse_numbers_bonus.c \
	srcs/parsing/parse_objects_bonus.c \
	srcs/parsing/parse_modifiers_bonus.c \
	srcs/parsing/parse_scene_bonus.c \
	srcs/parsing/parse_utils_bonus.c \
	srcs/parsing/str_to_double_bonus.c \
	srcs/output/cleanup_bonus.c \
	srcs/output/hooks_bonus.c \
	srcs/output/mlx_init_bonus.c \
	srcs/output/render_loop_bonus.c \
	srcs/vec3_math_bonus.c \
	srcs/vec3_ops_bonus.c \
	srcs/render/ray_bonus.c \
	srcs/render/camera_bonus.c \
	srcs/render/sphere_bonus.c \
	srcs/render/plane_bonus.c \
	srcs/render/cylinder_bonus.c \
	srcs/render/cone_bonus.c \
	srcs/render/normal_bonus.c \
	srcs/render/uv_bonus.c \
	srcs/render/checkerboard_bonus.c \
	srcs/render/tangent_bonus.c \
	srcs/render/bump_bonus.c \
	srcs/render/hit_object_bonus.c \
	srcs/render/hit_scene_bonus.c \
	srcs/render/lighting_bonus.c \
	srcs/render/shadow_bonus.c \
	srcs/render/render_pixel_bonus.c

OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

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

.PHONY : bonus
bonus: $(LIBFT) $(MLX) $(NAME_BONUS)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(MLX):
	@if [ ! -d "$(MLX_DIR)" ]; then \
		echo "minilibx not found: $(MLX_DIR)"; exit 1; \
	fi
	make -C $(MLX_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(LIBS) -o $(NAME)

$(NAME_BONUS): $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(BONUS_OBJS) $(LIBS) -o $(NAME_BONUS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(BONUS_OBJS)
	make -C $(LIBFT_DIR) clean
	@if [ -d "$(MLX_DIR)" ]; then make -C $(MLX_DIR) clean; fi

.PHONY: fclean
fclean: clean
	rm -f $(NAME) $(NAME_BONUS)
	make -C $(LIBFT_DIR) fclean

.PHONY: re
re: fclean all
