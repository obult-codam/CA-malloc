# General
CC = clang
INCL = -I ../lib
CFLAGS = -Wall -Wextra -g --coverage -O0
LIBFT = ../Libft/libft.a
OB_STRAT_DIR = ../obj/strategy

# Unit tests
STRATEGY = array
UTEST = unittest
UTEST_TST = unit_tests.o
UTEST_SRCS = ../src/strategy/${STRATEGY}.c
UTEST_OBJS = $(patsubst ../src/%.c, ../obj/%.o, $(UTEST_SRCS)) ${UTEST_TST}
UTEST_GCDANO = $(UTEST_OBJS:.o=.gcda) $(UTEST_OBJS:.o=.gcno)

# Full test
FULLTEST = fulltest
FT_SRC = full_tests.c
FT_OBJ = $(FT_SRC:.c=.o)
FT_GCDANO = $(FT_SRC:.c=.gcda) $(FT_SRC:.c=.gcno)
FT_SYM = libft_malloc.so
MALLIB = libft_malloc_arm64_Darwin.so


all: run coverage clean

%.o: %.c
	$(CC) $(CFLAGS) ${INCL} -c $< -o $@

../obj/%.o: ../src/%.c
	$(CC) $(CFLAGS) ${INCL} -c $< -o $@

${UTEST}:		${OB_STRAT_DIR} ${UTEST_OBJS} ${LIBFT}
					@${CC} $(UTEST_OBJS) ${LIBFT} $(CFLAGS) ${INCL} -o $@

${FULLTEST}:	${FT_OBJ} ${LIBFT} ${MALLIB}
					@$(CC) -L. ${MALLIB} ${FT_OBJ} ${LIBFT} ${CFLAGS} ${INCL} -o $@

${LIBFT}:
					@${MAKE} bonus -C ../Libft --no-print-directory 2>&1

${MALLIB}:			../${MALLIB}
					@cp ../${MALLIB} .

../${MALLIB}:
					${MAKE} -C ..

${OB_STRAT_DIR}:
					mkdir -p $(dir $@)

run:	${FULLTEST} ${UTEST}
			./${FULLTEST}
			./${UTEST}

coverage:
	@geninfo ../obj -o cov.info 2>&1
	@genhtml cov.info -o temp 2>&1

clean:
	@${MAKE} clean -C ..
	@rm -rf *.o cov_html
	@rm -rf ${UTEST_GCDANO} ${UTEST_OBJS}
	@rm -rf ${FT_GCDANO} ${FT_OBJ}
	@rm -rf cov.info

fclean:	clean
	@${MAKE} fclean -C ..
	@rm -rf temp
	@rm -rf ${MALLIB}
	@rm -rf ${UTEST} ${FULLTEST}

re: fclean all
