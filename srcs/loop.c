#include "minishell.h"

extern int	g_ret;

void	cmd_loop(t_all *all)
{
	while (all->cmd)
	{
		if (check_cap(all->cmd->arg[0], "echo"))
			builtin_echo(all->cmd);
		else if (!ft_strcmp(all->cmd->arg[0], "cd"))
			cd_pipe(all->fd.fd[1], all->cmd->arg[1]);
		else if (check_cap(all->cmd->arg[0], "pwd"))
		{
			printf("%s\n", getcwd(NULL, 0));
			g_ret = 0;
		}
		else if (!ft_strcmp(all->cmd->arg[0], "export"))
			builtin_export(all->cmd, &all->envl);
		else if (!ft_strcmp(all->cmd->arg[0], "unset"))
			builtin_unset(all->cmd, &all->envl);
		else if (check_cap(all->cmd->arg[0], "env"))
			builtin_env(all->envl, 0);
		else if (!ft_strcmp(all->cmd->arg[0], "exit"))
			exit(CMD_EXIT);
		else
			ft_exec(all->cmd, all->envl);
		all->cmd = free_next(all->cmd);
	}
}

int	pid_child_loop(t_all *all, int i)
{
	all->cmd = init_cmd(all->pid.pipe_cmd[i], all->envl);
	if (!all->cmd || all->cmd->ret > 0)
		return (1);
	set_fd(&all->fd, all->pid.total - 1, i);
	cmd_loop(all);
	exit(EXIT_SUCCESS);
}

void	pid_parent_loop(t_all *all, int i)
{
	int		status;

	waitpid(all->pid.pid[i], &status, 0);
	close(all->fd.fd[1]);
	exit_status(all, status);
	if (all->pid.total > 1)
	{
		if (i > 0)
			close(all->fd.prev_fd);
		all->fd.prev_fd = all->fd.fd[0];
	}
}

void	pid_loop(t_all *all)
{
	int		i;

	i = -1;
	while (++i < all->pid.total)
	{
		pipe(all->fd.fd);
		all->pid.pid[i] = fork();
		if (all->pid.pid[i] < 0)
			err_msg("fork failed\n");
		if (all->pid.pid[i] == 0)
		{
			if (pid_child_loop(all, i))
				continue ;
		}
		else
			ft_exec(cmd, envl);
		cmd = free_next(cmd);
			pid_parent_loop(all, i);
	}
	close(all->fd.prev_fd);
	if (all->pid.total > 1)
		dup_close(all->fd.fd_bu[0], STDIN_FILENO);
	free(all->pid.pid);
}
