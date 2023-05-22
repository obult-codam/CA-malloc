ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME		= libft_malloc_${HOSTTYPE}.so
SYMNAME 	:= libft_malloc.so
TESTNAME	:= malloctest
OBJ			= global_const malloc free show_alloc_mem
OBJS		= $(addsuffix .o, $(addprefix obj/, ${OBJ}))
CC			= gcc
RM			= rm -f
HEADER		= -I lib/ 
CFLAGS		= -fsanitize=address #-Wall -Wextra -Werror
LIBFT		= Libft/libft.a


all:		${NAME}

obj/%.o:	src/%.c
				@mkdir -p $(dir $@)
				$(CC) $(CFLAGS) $(RL_I) $(HEADER) -c -o $@ $<

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
				$(info ************  malloc Ready!)

test:	${NAME}
	gcc src/testmain.c -L. ${SYMNAME} ${LIBFT} ${HEADER} -o ${TESTNAME}

# fast test
ft: re test
	./malloctest

.PHONY: all clean fclean re cleanft