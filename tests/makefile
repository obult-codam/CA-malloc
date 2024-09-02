# General
CC = clang
INCL = -I ../lib
CFLAGS = -Wall -Wextra -g --coverage -O0
LIBFT = ../Libft/libft.a
OB_STRAT_DIR = ../obj/strategy
MUTE		= > /dev/null
MUTE_ALL	= 2>&1 > /dev/null

# Unit tests vars:
STANDARD_UT = obj/unit_tests.o

# array
ARRAY_UT = array.tst
ARRAY_UT_SRCS = ../src/strategy/array.c
ARRAY_UT_OBJS = $(patsubst ../src/%.c, ../obj/%.o, $(ARRAY_UT_SRCS)) ${STANDARD_UT}
ARRAY_UT_GCDANO = $(ARRAY_UT_OBJS:.o=.gcda) $(ARRAY_UT_OBJS:.o=.gcno)

# linked
LINKED_UT = linked.tst
LINKED_UT_SRCS = ../src/strategy/linked.c
LINKED_UT_OBJS = $(patsubst ../src/%.c, ../obj/%.o, $(LINKED_UT_SRCS)) ${STANDARD_UT}
LINKED_UT_GCDANO = $(LINKED_UT_OBJS:.o=.gcda) $(LINKED_UT_OBJS:.o=.gcno)

# multi_cell
MCELL_UT = multi_cell.tst
MCELL_UT_SRCS = ../src/strategy/multi_cell.c
MCELL_UT_OBJS = $(patsubst ../src/%.c, ../obj/%.o, $(MCELL_UT_SRCS)) obj/multi_cell_ut.o
MCELL_UT_GCDANO = $(MCELL_UT_OBJS:.o=.gcda) $(MCELL_UT_OBJS:.o=.gcno)

# yolo
YOLO_UT = yolo.tst
YOLO_UT_SRCS = ../src/strategy/yolo.c
YOLO_UT_OBJS = $(patsubst ../src/%.c, ../obj/%.o, $(YOLO_UT_SRCS)) obj/yolo_tests.o
YOLO_UT_GCDANO = $(YOLO_UT_OBJS:.o=.gcda) $(YOLO_UT_OBJS:.o=.gcno)

# large
LARGE_UT = large.tst
LARGE_UT_SRCS =
LARGE_UT_OBJS = $(patsubst ../src/%.c, ../obj/%.o, $(LARGE_UT_SRCS)) obj/large_tests.o
LARGE_UT_GCDANO = $(LARGE_UT_OBJS:.o=.gcda) $(LARGE_UT_OBJS:.o=.gcno)

# Full test
FULLTEST = full.tst
FT_OBJ = obj/full_tests.o
FT_GCDANO = $(FT_OBJ:.o=.gcda) $(FT_OBJ:.o=.gcno)
FT_SYM = libft_malloc.so
MALLIB = libft_malloc_arm64_Darwin.so

all: run coverage clean

obj/%.o: %.c
	@$(CC) $(CFLAGS) ${INCL} -c $< -o $@ ${MUTE_ALL}

../obj/%.o: ../src/%.c
	@$(CC) $(CFLAGS) ${INCL} -c $< -o $@ ${MUTE_ALL}

tests-all: units-all full-all
			${MAKE} coverage

# Unit tests:
units-all:		${ARRAY_UT} ${LINKED_UT} ${MCELL_UT} ${YOLO_UT}
				./${ARRAY_UT} ${MUTE}
				./${LINKED_UT} ${MUTE}
				./${MCELL_UT} ${MUTE}
				./${YOLO_UT} ${MUTE}

${ARRAY_UT}:	${OB_STRAT_DIR} ${ARRAY_UT_OBJS} ${LIBFT}
					@${CC} $(ARRAY_UT_OBJS) ${LIBFT} $(CFLAGS) ${INCL} -o $@

${LINKED_UT}:	${OB_STRAT_DIR} ${LINKED_UT_OBJS} ${LIBFT}
					@${CC} $(LINKED_UT_OBJS) ${LIBFT} $(CFLAGS) ${INCL} -o $@

${MCELL_UT}:	${OB_STRAT_DIR} ${MCELL_UT_OBJS} ${LIBFT}
					@${CC} $(MCELL_UT_OBJS) ${LIBFT} $(CFLAGS) ${INCL} -o $@

${YOLO_UT}:		${OB_STRAT_DIR} ${YOLO_UT_OBJS} ${LIBFT}
					@${CC} $(YOLO_UT_OBJS) ${LIBFT} $(CFLAGS) ${INCL} -o $@

${LARGE_UT}:	${OB_STRAT_DIR} ${LARGE_UT_OBJS} ${LIBFT}
					@${CC} $(LARGE_UT_OBJS) ${LIBFT} $(CFLAGS) ${INCL} -o $@

# Full test needs to build the library with the right malloc, run the test and then clean the lib to build the next with the next strat..
# Full tests:
full-all:
				@${MAKE} full-array
				@${MAKE} full-linked
				@${MAKE} full-multi_cell
				@${MAKE} full-yolo

full-%:			create-lib-%
				@${MAKE} ${FULLTEST}
				@./${FULLTEST} ${MUTE}

create-lib-%:	remove-lib
				@${MAKE} test -C .. STRATEGY=$*

remove-lib:
					@${RM} ${MALLIB} ${MUTE_ALL}
					@${MAKE} lib-remove -C .. ${MUTE_ALL}

${FULLTEST}:	${FT_OBJ} ${LIBFT} ${MALLIB}
					@$(CC) -L. ${MALLIB} ${FT_OBJ} ${LIBFT} ${CFLAGS} ${INCL} -o $@ ${MUTE_ALL}

${MALLIB}:			../${MALLIB}
					@cp ../${MALLIB} .

${LIBFT}:
					@${MAKE} bonus -C ../Libft --no-print-directory 2>&1

../${MALLIB}:
					${MAKE} -C ..

${OB_STRAT_DIR}:
					mkdir -p $(dir $@)

coverage:
	@geninfo ../obj -o cov.info 2>&1
	@genhtml cov.info -o temp 2>&1

clean: clean-array clean-linked clean-multi_cell clean-yolo
	@rm -rf obj/*.o obj/*.gc* *.ut cov_html
	@rm -rf ${ARRAY_UT_GCDANO} ${ARRAY_UT_OBJS}
	@rm -rf ${FT_GCDANO} ${FT_OBJ}
	@rm -rf cov.info

clean-%:
		@${MAKE} clean -C .. STRATEGY=$*

fclean:	clean
	@${MAKE} fclean -C ..
	@rm -rf ${MALLIB}
	@${RM} ${ARRAY_UT} ${LINKED_UT} ${MCELL_UT} ${YOLO_UT} ${FULLTEST}

clean-coverage:
	@rm -rf temp

re: fclean all

.PHONY: create-lib-% remove-lib coverage clean clean-% fclean clean-coverage re
.SUFFIXES:
