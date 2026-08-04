NAME = miniRT
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDE = -I./include -I$(MLX_DIR) -I./libft

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)

LIBFT_DIR = ./libft
MLX_DIR = ./minilibx-linux

LIBFT = $(LIBFT_DIR)/libft.a
MLX = $(MLX_DIR)/libmlx.a

LIBS = -L$(LIBFT_DIR) -L$(MLX_DIR)\
		-lft -lmlx -lXext -lX11 -lm

.PHONY : all
all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(LIBS) -o $(NAME)

.PHONY: clean
clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

.PHONY: fclean
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

.PHONY: re
re: fclean all