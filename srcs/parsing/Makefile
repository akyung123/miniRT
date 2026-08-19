CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDE = -I../../includes -I../../libft

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all clean fclean re test suite

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean

re: fclean all

# 파싱 결과를 직접 확인하는 자체 테스트 (제출 대상 아님, mlx 불필요)
test:
	make -C ../../libft
	$(CC) $(CFLAGS) $(INCLUDE) $(SRCS) ../error.c ../../tests/parse_test.c \
		-L../../libft -lft -lm -o ../../tests/parse_test
	../../tests/parse_test ../../scenes/valid/example.rt

# scenes/valid, scenes/invalid 전체를 돌려서 기대한 성공/실패와 맞는지 확인
suite: test
	sh ../../tests/run_scenes.sh
