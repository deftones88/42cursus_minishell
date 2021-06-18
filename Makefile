CC = gcc
CFLAGS = -Wall -Wextra #-Werror

NAME = minishell
LIBFT = libft.a
INCLUDE = includes/
SRCS = $(wildcard ./srcs/*.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

all : $(NAME)

$(NAME) : $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $@ -L. -lft

$(LIBFT) :
	make all -C ./libft
	cp ./libft/libft.h $(INCLUDE)
	cp ./libft/libft.a ./

clean :
	make clean -C ./libft
	rm -f $(OBJS) $(DEPS)

fclean : clean
	rm -f $(LIBFT) ./libft/libft.a
	rm -f $(NAME)

re : fclean all

.c.o :
	$(CC) $(CFLAGS) -MD -c -o $@ $< -I$(INCLUDE) -L. -lft

-include $(DEPS)

.PHONY: all clean fclean re
