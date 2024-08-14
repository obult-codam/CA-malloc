NAME = tp
CC = clang
CFLAGS = -Wall -Wextra -g --coverage -I ../lib
LDFLAGS = -fprofile-instr-generate
LIBFT		= ../Libft/libft.a

# The module under test
MODULE = ../src/strategy
TEST_SRCS = tests.c
TEST_SRCS += ../src/strategy/linked.c # the module
# TEST_SRCS += $(wildcard ../src/*.c) # the higher level implementation.
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_GCDANO = $(TEST_SRCS:.c=.gcda) $(TEST_SRCS:.c=.gcno)
# TEST_PRGS = $(TEST_SRCS:.c=)

all: ${NAME} coverage clean

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%: %.o
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

${NAME}:	${TEST_OBJS}
				@${MAKE} bonus -C ../Libft --no-print-directory  > output.log 2>&1
				@${CC} $(TEST_OBJS) ${LIBFT} $(CFLAGS) -o $@

coverage:
	@./${NAME}
	@geninfo ../src/ -o cov.info > output.log 2>&1
	@genhtml cov.info -o temp > output.log 2>&1

clean:
	@rm -rf *.o $(TEST_PRGS) *.profdata coverage.info cov_html
	@rm -rf test.main.dSYM
	@rm -rf ${TEST_GCDANO} ${TEST_OBJS}
	@rm -rf cov.info
	@rm -rf *.gcov

fclean:	clean
	@rm -rf temp
	@rm -rf ${NAME}

re: fclean all
