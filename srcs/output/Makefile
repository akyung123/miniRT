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

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

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
		../parsing/*.c ../../tests/render_pixel_stub.c \
		-L../../libft -L$(MLX_DIR) -lft $(MLX_LIBS) -lm \
		-o ../../tests/output_test
	../../tests/output_test ../../scenes/valid/example.rt
