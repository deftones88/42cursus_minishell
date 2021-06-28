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

void set_term(void)
{
	struct termios	t_new;

	tcgetattr(0, &t_new);
	t_new.c_lflag &= ~(ECHOCTL);
	tcsetattr(0, TCSANOW, &t_new);
}
