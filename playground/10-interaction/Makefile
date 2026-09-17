NAME = miniRT
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

# 9단계: 팀원의 진짜 파서를 그대로 링크한다 (소스는 건드리지 않는다)
# 오브젝트는 루트 빌드(ASAN 없음)와 섞이지 않게 여기 .team/ 에 따로 만든다
TEAM_DIR = ../../srcs
TEAM_SRCS = $(wildcard $(TEAM_DIR)/parsing/*.c) $(TEAM_DIR)/error.c
TEAM_OBJS = $(addprefix .team/, $(notdir $(TEAM_SRCS:.c=.o)))

LIBFT_DIR = ../../libft
LIBFT = $(LIBFT_DIR)/libft.a

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
	MLX_DIR = ../../minilibx_macos_opengl
	MLX_LIBS = -lmlx -framework OpenGL -framework AppKit
else
	MLX_DIR = ./minilibx-linux
	MLX_LIBS = -lmlx -lXext -lX11
endif

MLX = $(MLX_DIR)/libmlx.a
# 순서 중요: 내 includes/miniRT.h 가 먼저 잡혀야 한다
# 파서의 parsing.h 는 자기 폴더의 루트 miniRT.h 를 끌어오는데, 두 헤더의 구조체 배치가 같아서 링크해도 안전하다
INCLUDE = -I./includes -I$(MLX_DIR) -I../../includes -I$(LIBFT_DIR)

LIBS = -L$(LIBFT_DIR) -L$(MLX_DIR) -lft $(MLX_LIBS) -lm

.PHONY : all
all: $(LIBFT) $(MLX) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(MLX):
	make -C $(MLX_DIR)

%.o: %.c includes/miniRT.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

.team/%.o: $(TEAM_DIR)/parsing/%.c
	@mkdir -p .team
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

.team/error.o: $(TEAM_DIR)/error.c
	@mkdir -p .team
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS) $(TEAM_OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(TEAM_OBJS) $(LIBS) -o $(NAME)

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -rf .team
	make -C $(LIBFT_DIR) clean
	make -C $(MLX_DIR) clean

.PHONY: fclean
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

.PHONY: re
re: fclean all
