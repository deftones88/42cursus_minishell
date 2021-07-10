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
	if (total > 1)
	{
		if (i > 0)
			dup_close(fd->prev_fd, STDIN_FILENO);
		if (i < total - 1)
			dup_close(fd->fd[1], STDOUT_FILENO);
		else
			dup_close(fd->fd_bu[1], STDOUT_FILENO);
	}
}

void	exit_status(t_fd fd, int status, t_list *envl)
{
	char	dir[100];

	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == CMD_EXIT)
		{
			printf("exit\n");
			exit(EXIT_SUCCESS);
		}
		else if (WEXITSTATUS(status) == CMD_CD)
		{
			read(fd.fd[0], &dir, 99);
			builtin_cd(dir, &envl);
		}
		else
			g_ret = WEXITSTATUS(status);
	}
}
