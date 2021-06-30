#ifndef SIG_HANDLER_H
# define SIG_HANDLER_H

# include <stdio.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
// # include <termcap.h>
# include <curses.h>
# include <term.h>
# include "libft.h"

void	sig_handler(int sig);
void	set_termios(int i);
void	get_cursor_position(int *col, int *rows);
void	set_line(int col, int row, char *cm, char *ce);
int		ft_putchar(int c);
// void	set_termcap(void);

#endif
