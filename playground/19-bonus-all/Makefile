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

MLX_LINUX_ROOT = ../../minilibx-linux
MLX_LINUX_REPO = https://github.com/42Paris/minilibx-linux.git

.PHONY : all
all: $(LIBFT) $(MLX) $(NAME)

.PHONY : bonus
bonus: $(LIBFT) $(MLX) $(NAME_BONUS)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(MLX):
	@if [ "$(UNAME_S)" != "Darwin" ] && [ ! -d "$(MLX_DIR)" ]; then \
		if [ -L "$(MLX_DIR)" ]; then rm -f "$(MLX_DIR)"; fi; \
		if [ -d "$(MLX_LINUX_ROOT)" ]; then \
			ln -sfn "$(MLX_LINUX_ROOT)" "$(MLX_DIR)"; \
		else \
			git clone --depth 1 $(MLX_LINUX_REPO) "$(MLX_DIR)"; \
		fi; \
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
	rm -f $(NAME) $(NAME_BONUS) dump_ppm dump_ppm_bonus
	rm -f tests/parse_test tests/parse_test_bonus
	make -C $(LIBFT_DIR) fclean

.PHONY: re
re: fclean all

# ---------- 검사용 (제출 대상 아님) ----------
# mlx 없이 씬을 PPM 으로 뽑아 렌더 결과를 파일로 비교할 때 쓴다.
# mlx 를 쓰는 main/output 은 빼고 파서+렌더러만 링크한다.
DUMP_SRCS = $(filter-out srcs/main.c srcs/output/%, $(SRCS)) dump_ppm.c
DUMP_BONUS_SRCS = $(filter-out srcs/main_bonus.c srcs/output/%, $(BONUS_SRCS)) \
	dump_ppm_bonus.c

.PHONY: dump_ppm
dump_ppm: $(LIBFT)
	$(CC) $(CFLAGS) -I./includes -I./libft $(DUMP_SRCS) \
		-L$(LIBFT_DIR) -lft -lm -o dump_ppm

.PHONY: dump_ppm_bonus
dump_ppm_bonus: $(LIBFT)
	$(CC) $(CFLAGS) -I./includes -I./libft $(DUMP_BONUS_SRCS) \
		-L$(LIBFT_DIR) -lft -lm -o dump_ppm_bonus

# 파서 회귀 테스트 바이너리 (mlx 불필요)
TEST_SRCS = srcs/error.c srcs/vec3_math.c srcs/vec3_ops.c \
	$(filter srcs/parsing/%, $(SRCS)) tests/parse_test.c
TEST_BONUS_SRCS = srcs/error_bonus.c srcs/vec3_math_bonus.c srcs/vec3_ops_bonus.c \
	$(filter srcs/parsing/%, $(BONUS_SRCS)) tests/parse_test_bonus.c

.PHONY: test
test: $(LIBFT)
	$(CC) $(CFLAGS) -I./includes -I./libft $(TEST_SRCS) \
		-L$(LIBFT_DIR) -lft -lm -o tests/parse_test
	sh tests/run_scenes.sh

.PHONY: test_bonus
test_bonus: $(LIBFT)
	$(CC) $(CFLAGS) -I./includes -I./libft $(TEST_BONUS_SRCS) \
		-L$(LIBFT_DIR) -lft -lm -o tests/parse_test_bonus
	sh tests/run_scenes_bonus.sh
