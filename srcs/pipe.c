#include "minishell.h"

extern int	g_ret;

void	dup_close(int fd, int dup)
{
	dup2(fd, dup);
	close(fd);
}

void	set_fd(t_fd *fd, int total, int i)
{
	close(fd->fd[0]);
	if (total)
	{
		if (i > 0)
			dup_close(fd->prev_fd, STDIN_FILENO);
		if (i < total)
			dup_close(fd->fd[1], STDOUT_FILENO);
		else
			dup_close(fd->fd_bu[1], STDOUT_FILENO);
	}
}

void	cd_pipe(int fd, char *arg)
{
	write(fd, arg, (int)ft_strlen(arg));
	exit(CMD_CD);
}

//////////////////////////
void	exit_status(t_all *all, int status)
{
	char	dir[100];

	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == CMD_EXIT)
		{
			printf("exit\n");
			tcsetattr(STDIN_FILENO, TCSANOW, &all->t_old);
			exit(EXIT_SUCCESS);
		}
		else if (WEXITSTATUS(status) == CMD_CD)
		{
			ft_bzero(dir, 100);
			read(all->fd.fd[0], &dir, 99);
			builtin_cd(dir, &all->envl);
		}
		else
		g_ret = WEXITSTATUS(status);
	}
}
