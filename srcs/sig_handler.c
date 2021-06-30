#include "sig_handler.h"

void sig_handler(int sig)
{
	if (sig)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void set_termios(int i)
{
	struct termios	t_new;

	tcgetattr(0, &t_new);
	if (!i)
		t_new.c_lflag &= ~(ECHOCTL);
	else
	{
		t_new.c_lflag &= ~(ICANON | ECHO);
		t_new.c_cc[VMIN] = 1;
		t_new.c_cc[VTIME] = 0;
	}
	tcsetattr(0, TCSANOW, &t_new);
}

void	get_cursor_position(int *col, int *rows)
{
	int		a;
	int		i;
	char	buf[4];

	a = 0;
	i = -1;
	write(1, "\033[6n", 4);
	read(1, buf, 4);
	while (buf[++i])
	{
		if (buf[i] >= '0' && buf[i] <= '9')
		{
			if (a == 0)
				*rows = ft_atoi(&buf[i]) - 1;
			else
				*col = ft_atoi(&buf[i]) - 1;
			a++;
		}
	}
}

void	set_line(int col, int row, char *cm, char *ce)
{
	tputs(tgoto(cm, col, row - 1), 1, ft_putchar);
	tputs(ce, 1, ft_putchar);
}

int		ft_putchar(int c)
{
	write(1, &c, 1);
	return (0);
}
/*
void set_termcap(void)
{
	char	*cm;
	char	*ce;
	int		col;
	int		row;

	tgetent(NULL, "xterm");
	cm  = tgetstr("cm", NULL);
	ce  = tgetstr("ce", NULL);
	get_cursor_position(&col, &row);
	delete_line(col, row, cm, ce);
}
*/
