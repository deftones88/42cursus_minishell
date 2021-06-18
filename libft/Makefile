CC = gcc
CFLAGS = -Wall -Wextra -Werror

NAME = libft.a
SRCS = $(wildcard ./ft_[^l]*.c)
SRCS_B = $(wildcard ./ft_l*.c)
OBJS = $(SRCS:.c=.o)
OBJS_B = $(SRCS_B:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	@ar -cr $@ $(OBJS)

bonus : $(NAME) $(OBJS_B)
	ar -cr $(NAME) $(OBJS_B)

clean :
	rm -f $(OBJS) $(OBJS_B)

fclean : clean
	rm -f $(NAME)

re : fclean all

.c.o :
	$(CC) $(CFLAGS) -c -o $@ $<
