NAME = miniRT
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = $(wildcard srcs/*.c srcs/parsing/*.c srcs/output/*.c)
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
