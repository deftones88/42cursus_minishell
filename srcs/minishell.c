/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:16:37 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/09 17:30:16 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_ret = 0;

int		main(int argc, char **argv, char **envp)
{
	char	*line;
	t_all	*all;
	// t_cmd	*cmd;

	show_logo();
	all = init_all(envp);
	while(1)
	{
		line = readline("minishell$ ");
		if (line && line[0] != 0)
		{
			add_history(line);
			init_pid(&all->pid, line);
			for (int i = 0; i < all->pid.total; i++)
			{
				pipe(all->fd.fd);
				all->pid.pid[i] = fork();
				if (all->pid.pid[i] < 0)
					err_msg("fork failed\n");
				if (all->pid.pid[i] == 0)
				{
					all->cmd = init_cmd(all->pid.pipe_cmd[i], all->envl);
					if (!all->cmd || all->cmd->ret > 0)
						continue ;
					set_fd(&all->fd, all->pid.total - 1, i);
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
					exit(EXIT_SUCCESS);
				}
				else
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
			}
			close(all->fd.fd[0]);
			if (all->pid.total > 1)
				dup_close(all->fd.fd_bu[0], STDIN_FILENO);
			free(all->pid.pid);
		}
		else if (line == NULL)
		{
			set_termcap(0);
			printf("minishell$ exit\n");
			tcsetattr(STDIN_FILENO, TCSANOW, &all->t_old);
			exit(EXIT_SUCCESS);
		}
		free(line);
	}
	return (0);
}
