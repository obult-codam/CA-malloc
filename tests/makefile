# General
CC = clang
INCL = -I ../lib
CFLAGS = -Wall -Wextra -g --coverage -fsanitize=address
LIBFT = ../Libft/libft.a

# Unit tests
UTEST = unittest
UTEST_SRCS = unit_tests.c
UTEST_SRCS += ../src/strategy/linked.c # the module
UTEST_OBJS = $(UTEST_SRCS:.c=.o)
UTEST_GCDANO = $(UTEST_SRCS:.c=.gcda) $(UTEST_SRCS:.c=.gcno)

# Full test
FULLTEST = fulltest
FT_SRC = full_tests.c ../src/core.c ../src/strategy/linked.c
FT_OBJ = $(FT_SRC:.c=.o)
FT_GCDANO = $(FT_SRC:.c=.gcda) $(FT_SRC:.c=.gcno)



all: ${UTEST} coverage clean

%.o: %.c
	$(CC) $(CFLAGS) ${INCL} -c $< -o $@

${UTEST}:		${UTEST_OBJS} ${LIBFT}
					@${CC} $(UTEST_OBJS) ${LIBFT} $(CFLAGS) ${INCL} -o $@

${FULLTEST}:	${FT_OBJ} ${LIBFT}
					@$(CC) ${FT_OBJ} ${LIBFT} ${CFLAGS} ${INCL} -o $@

${LIBFT}:
					@${MAKE} bonus -C ../Libft --no-print-directory 2>&1

ft:				${FULLTEST}
					./${FULLTEST}

coverage:
	@geninfo ../src/ -o cov.info 2>&1
	@genhtml cov.info -o temp 2>&1

clean:
	@rm -rf *.o cov_html
	@rm -rf test.main.dSYM
	@rm -rf ${UTEST_GCDANO} ${UTEST_OBJS}
	@rm -rf ${FT_GCDANO} ${FT_OBJ}
	@rm -rf cov.info

fclean:	clean
	@rm -rf temp
	@rm -rf ${COPY_LIB}
	@rm -rf ${UTEST} ${FULLTEST}

re: fclean all
