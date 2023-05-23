ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME		= libft_malloc_${HOSTTYPE}.so
SYMNAME 	:= libft_malloc.so
TESTNAME	:= malloctest
OBJ			= global_const malloc free show_alloc_mem \
				reporting \
				zone_cleanup zone_create zone_find \
				ll_alloc_cleanup ll_alloc_create ll_alloc_find

OBJS		= $(addsuffix .o, $(addprefix obj/, ${OBJ}))
CC			= gcc
RM			= rm -f
HEADER		= -I lib/ -I .
CFLAGS		= #-fsanitize=address #-Wall -Wextra -Werror
LIBFT		= Libft/libft.a


all:		${NAME}

obj/%.o:	src/%.c
				@mkdir -p $(dir $@)
				$(CC) -fPIC $(CFLAGS) $(RL_I) $(HEADER) -c -o $@ $<

clean:
				@${RM} ${OBJS} \
				$(info ************  malloc Clean)

fclean:		clean
				@${MAKE} fclean -C Libft --no-print-directory
				@${RM} ${NAME} ${SYMNAME} ${TESTNAME}
				$(info ************  malloc Removed)

re:			fclean all

${NAME}:	${OBJS}
				@${MAKE} bonus -C Libft --no-print-directory
				@${CC} -shared $(OBJS) $(LIBFT) $(CFLAGS) -o $@
				@ln -s ${NAME} ${SYMNAME}
				@export LD_LIBRARY_PATH=$(pwd)
				$(info ************  malloc Ready!)

test:	${NAME}
	gcc src/testmain.c -L. ${SYMNAME} ${LIBFT} ${HEADER} -o ${TESTNAME}

# fast test
ft: re test
	./malloctest

.PHONY: all clean fclean re cleanft