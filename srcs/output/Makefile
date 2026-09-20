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

.PHONY: all clean fclean re test

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean

re: fclean all

# render_pixel 스텁(그라디언트)으로 output 모듈만 독립 테스트
# (실제 render_pixel 붙기 전까지, mlx 창 뜨는지/이벤트 루프 도는지 확인용)
test:
	make -C ../../libft
	make -C $(MLX_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(SRCS) ../error.c ../main.c \
		$(PARSING_SRCS) ../../tests/render_pixel_stub.c \
		-L../../libft -L$(MLX_DIR) -lft $(MLX_LIBS) -lm \
		-o ../../tests/output_test
	../../tests/output_test ../../scenes/valid/example.rt

# 이미지 버퍼 캐싱(render_all) vs mlx_pixel_put 매 픽셀 호출 속도 비교 데모
# (실제 render_pixel로 링크 - 화면에 진짜 렌더링 결과가 뜸)
perf:
	make -C ../../libft
	make -C $(MLX_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(SRCS) ../error.c \
		$(PARSING_SRCS) $(RENDER_SRCS) \
		../../tests/perf_test.c \
		-L../../libft -L$(MLX_DIR) -lft $(MLX_LIBS) -lm \
		-o ../../tests/perf_test
	../../tests/perf_test ../../scenes/valid/example.rt
