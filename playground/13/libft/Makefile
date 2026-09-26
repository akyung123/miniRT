CC = cc
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)
SRCS = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c ft_strlcat.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c get_next_line.c ft_handle.c ft_printf_format.c printf_int.c ft_put_fuction.c printf_chars.c printf_unit.c ft_printf.c ft_vftprintf.c printf_hex.c pritnf_pointer.c ft_strcmp.c
SRCS_BONUS = ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c ft_lstclear_bonus.c ft_lstdelone_bonus.c ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c ft_lstnew_bonus.c ft_lstsize_bonus.c 
OBJS = $(SRCS:%.c=%.o)
OBJS_BONUS = $(SRCS_BONUS:%.c=%.o)
HEADER = libft.h get_next_line.h ft_printf_bonus.h
INCLUDES= -I.

NAME = libft.a

.PHONY: all
all : $(NAME)

$(NAME): $(OBJS)
	@ar rcs $@ $^
	
$(OBJS): %.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: bonus
bonus : $(OBJS) $(OBJS_BONUS)
	@ar rcs $(NAME) $^

$(OBJS_BONUS): %.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(OBJS_BONUS)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)

.PHONY: re
re: fclean all

