export LD_LIBRARY_PATH=$(pwd)

ifndef STRATEGY
	STRATEGY := array
endif

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME		:= libft_malloc_${HOSTTYPE}.so
SYMNAME 	:= libft_malloc.so
SRC			:= src/core.c
SRC			+= src/strategy/${STRATEGY}.c

OBJS		:= $(patsubst src/%.c, obj/%.o, $(SRC))
GCDANO		:= $(OBJS:.o=.gcda) $(OBJS:.o=.gcno)
CC			:= clang
RM			:= rm -rf
HEADER		:= -I lib/ -I . -I ${STRATEGY}/
CFLAGS		:= -g -Wall -Wextra -Werror --coverage -O0
LIBFT		:= Libft/libft.a
MUTE		:=  2>&1 > /dev/null

all:		${NAME}

obj/%.o:	src/%.c
				@mkdir -p $(dir $@)
				$(CC) -fPIC $(CFLAGS) $(HEADER) -c -o $@ $< ${MUTE}

clean:
				@${RM} ${OBJS} ${GCDANO}
				$(info ************  malloc Clean)

fclean:		clean
				@${RM} ${OBJS}
				@${RM} ${NAME} ${SYMNAME} ${TESTNAME}
				$(info ************  malloc Removed)

clean-lib:
				@${MAKE} fclean -C Libft --no-print-directory ${MUTE}

re:			fclean all

${NAME}:	${OBJS} ${LIBFT}
				@${CC} -shared $(OBJS) $(LIBFT) ${LDFLAGS} $(CFLAGS) -o $@
				@ln -s ${NAME} ${SYMNAME}
				$(info ************  malloc Ready!)

${LIBFT}:
				@${MAKE} bonus -C Libft --no-print-directory ${MUTE}

lib-remove:
				${RM} ${SYMNAME} ${NAME}

.PHONY: all clean fclean clean-lib re lib-remove
.SUFFIXES:
