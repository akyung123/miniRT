NAME = color
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
	MLX_DIR = ../../minilibx_macos_opengl
	MLX_LIBS = -lmlx -framework OpenGL -framework AppKit
else
	MLX_DIR = ../../minilibx-linux
	MLX_LIBS = -lmlx -lXext -lX11
endif

# libft 는 2단계에 필요 없다. 파서를 붙이는 4단계부터 들어온다.
INCLUDE = -I./includes -I$(MLX_DIR)
LIBS = -L$(MLX_DIR) $(MLX_LIBS) -lm

TEST_NAME = color_test
TEST_SRCS = color.c tests/color_test.c

.PHONY: all
all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(LIBS) -o $(NAME)

# 창을 띄우지 않고 색 함수만 검사한다. mlx 링크 없음.
.PHONY: test
test:
	$(CC) $(CFLAGS) $(INCLUDE) $(TEST_SRCS) -lm -o $(TEST_NAME)
	./$(TEST_NAME)

.PHONY: clean
clean:
	rm -f $(OBJS)

.PHONY: fclean
fclean: clean
	rm -f $(NAME) $(TEST_NAME)

.PHONY: re
re: fclean all
