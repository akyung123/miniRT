CC = cc
CFLAGS = -Wall -Wextra -Werror -g

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
	MLX_DIR = ../../minilibx_macos_opengl
	MLX_LIBS = -lmlx -framework OpenGL -framework AppKit
else
	MLX_DIR = ../../minilibx-linux
	MLX_LIBS = -lmlx -lXext -lX11
endif

INCLUDE = -I../../includes -I$(MLX_DIR) -I../../libft

SRCS = cleanup.c \
	hooks.c \
	mlx_init.c \
	render_loop.c
OBJS = $(SRCS:.c=.o)

PARSING_SRCS = ../parsing/free_scene.c \
	../parsing/parse_elements.c \
	../parsing/parse_numbers.c \
	../parsing/parse_objects.c \
	../parsing/parse_scene.c \
	../parsing/parse_utils.c \
	../parsing/str_to_double.c

RENDER_SRCS = ../vec3_math.c \
	../vec3_ops.c \
	../render/ray.c \
	../render/camera.c \
	../render/sphere.c \
	../render/plane.c \
	../render/cylinder.c \
	../render/normal.c \
	../render/hit_scene.c \
	../render/lighting.c \
	../render/shadow.c \
	../render/render_pixel.c

.PHONY: all clean fclean re test perf ensure-mlx

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean

re: fclean all

# mlx 창이 뜨는지/이벤트 루프 도는지 확인 (실제 render_pixel 포함)
MLX_LINUX_REPO = https://github.com/42Paris/minilibx-linux.git

ensure-mlx:
	@if [ ! -d "$(MLX_DIR)" ]; then \
		if [ "$(UNAME_S)" = "Linux" ]; then \
			git clone --depth 1 $(MLX_LINUX_REPO) $(MLX_DIR); \
		else \
			echo "minilibx not found: $(MLX_DIR)"; \
			exit 1; \
		fi; \
	fi
	make -C $(MLX_DIR)

test: ensure-mlx
	make -C ../../libft
	$(CC) $(CFLAGS) $(INCLUDE) $(SRCS) ../error.c ../main.c \
		$(PARSING_SRCS) $(RENDER_SRCS) \
		-L../../libft -L$(MLX_DIR) -lft $(MLX_LIBS) -lm \
		-o ../../tests/output_test
	../../tests/output_test ../../scenes/valid/example.rt

# 이미지 버퍼 캐싱(render_all) vs mlx_pixel_put 매 픽셀 호출 속도 비교 데모
perf: ensure-mlx
	make -C ../../libft
	$(CC) $(CFLAGS) $(INCLUDE) $(SRCS) ../error.c \
		$(PARSING_SRCS) ../../tests/render_pixel_stub.c \
		../../tests/perf_test.c \
		-L../../libft -L$(MLX_DIR) -lft $(MLX_LIBS) -lm \
		-o ../../tests/perf_test
	../../tests/perf_test ../../scenes/valid/example.rt
