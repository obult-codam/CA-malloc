export LD_LIBRARY_PATH=$(pwd)

ifndef STRATEGY
	STRATEGY := strategy
endif

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME		= libft_malloc_${HOSTTYPE}.so
SYMNAME 	:= libft_malloc.so
STRATEGY_OBJ = $(patsubst src/$(STRATEGY)%.c,obj/$(STRATEGY)%.o,$(wildcard src/$(STRATEGY)/*.c))
OBJ			=	malloc free \
				show_alloc_mem \
				realloc \
				reporting \
				zone_create \
				zone_find

OBJS		= $(addsuffix .o, $(addprefix obj/, ${OBJ})) ${STRATEGY_OBJ}
CC			= clang
RM			= rm -rf
HEADER		= -I lib/ -I . -I ${STRATEGY}/
CFLAGS		= -g -Wall -Wextra -Werror --coverage
LIBFT		= Libft/libft.a

all:		${NAME}

obj/%.o:	src/%.c
				@mkdir -p $(dir $@)
				$(CC) -fPIC $(CFLAGS) $(HEADER) -c -o $@ $<

clean:
				@${RM} ${OBJS} \
				$(info ************  malloc Clean)

fclean:		clean
				@${MAKE} fclean -C Libft --no-print-directory 2>&1 > /dev/null
				@${RM} src/*.o obj/*
				@${RM} ${NAME} ${SYMNAME} ${TESTNAME}
				$(info ************  malloc Removed)

re:			fclean all

${NAME}:	${OBJS} ${LIBFT}
				${CC} -shared $(OBJS) $(LIBFT) ${LDFLAGS} $(CFLAGS) -o $@
				@ln -s ${NAME} ${SYMNAME}
				$(info ************  malloc Ready!)

${LIBFT}:
				@${MAKE} bonus -C Libft --no-print-directory 2>&1 > /dev/null

.PHONY: all clean fclean re cleanft