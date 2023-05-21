NAME		= malloctest
OBJ			= malloc testmain
OBJS		= $(addsuffix .o, $(addprefix obj/, ${OBJ}))
CC			= gcc
RM			= rm -f
HEADER		= -I lib/ 
CFLAGS		= #-Wall -Wextra -Werror
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
				@${RM} ${NAME}
				$(info ************  malloc Removed)

re:			fclean all

${NAME}:	${OBJS}
				@${MAKE} bonus -C Libft --no-print-directory
				@${CC} $(OBJS) $(LIBFT) $(CFLAGS) -o $@
				$(info ************  malloc Ready!)

.PHONY: all clean fclean re cleanft