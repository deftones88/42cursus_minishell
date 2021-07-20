/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:14:16 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/05 15:14:18 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_ret;

void	merge_path(t_cmd *cmd, char *str)
{
	char		**path;
	int			i;
	char		*tmp[2];
	struct stat	s;

	if (stat(cmd->arg[0], &s) == 0)
	{
		cmd->cmd = ft_strdup(cmd->arg[0]);
		return ;
	}
	path = ft_split(str, ":");
	i = -1;
	while (path[++i])
	{
		tmp[0] = ft_strjoin(path[i], "/");
		tmp[1] = ft_strjoin(tmp[0], cmd->arg[0]);
		free(tmp[0]);
		if (stat(tmp[1], &s) == 0)
		{
			cmd->cmd = tmp[1];
			break ;
		}
		free(tmp[1]);
	}
	free_all(path);
}

void	ft_exec_child(t_cmd *cmd, char **envp)
{
	if (cmd->delimit > 0)
		dup_close(cmd->delimit, STDIN_FILENO);
	if (cmd->redin > 0)
		dup_close(cmd->redin, STDIN_FILENO);
	if ((cmd->redout > 0 || cmd->append > 0))
		dup_close(cmd->redout + cmd->append + 1, STDOUT_FILENO);
	if (cmd->cmd && execve(cmd->cmd, cmd->arg, envp) == -1)
	{
		g_ret = errno;
		printf("Error executing : %s\n", strerror(errno));
	}
	else
	{
		g_ret = 127;
		printf("minishell: %s: command not found\n", cmd->arg[0]);
	}
	exit(g_ret);
}

void	ft_exec(t_cmd *cmd, t_list *envl)
{
	pid_t	pid;
	char	**envp;
	int		status;
	int		fd[2];

	fd[0] = dup(STDIN_FILENO);
	fd[1] = dup(STDOUT_FILENO);
	envp = list2arr(envl);
	merge_path(cmd, find_value(envl, "PATH"));
	pid = fork();
	if (pid < 0)
		err_msg("fork failed\n");
	if (pid == 0)
		ft_exec_child(cmd, envp);
	wait(&status);
	dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	if (WIFEXITED(status))
		exit(WEXITSTATUS(status));
}
