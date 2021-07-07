/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:14:16 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/05 15:14:18 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

extern int		g_ret;

void	merge_path(t_cmd *cmd, char *str)
{
	char		**path;
	int			i;
	char		*tmp[2];
	struct stat	s;

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

void	ft_exec(t_cmd *cmd, t_list *envl, int total)
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
	if (total > 1 && PRINT)
		printf("\e[33m----\t< exe > fork :\t%d (%4d)\t----\n\e[0m", getpid(), getppid());
	if (pid < 0)
		err_msg("fork failed\n");
	if (pid == 0)
	{
		if (cmd->delimit > 0)
		{
			dup2(cmd->delimit, STDIN_FILENO);
			close(cmd->delimit);
		}
		if (cmd->redin > 0)
		{
			dup2(cmd->redin, STDIN_FILENO);
			close(cmd->redin);
		}
		if ((cmd->redout > 0 || cmd->append > 0))
		{
			dup2(cmd->redout + cmd->append + 1, STDOUT_FILENO);
			close(cmd->redout + cmd->append + 1);
		}
		if (total > 1 && PRINT)
			printf("\t -. exe    /child/ :\t%d (%4d)\n", getpid(), getppid());
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
	// if (total > 1 && PRINT)
		// printf("\t< exe >\t\t>> waiting for child to exit\n");
	wait(&status);
	if (total > 1 && PRINT)
		printf("\t -. exe    /parent/ :\t%d (%4d)\n", getpid(), getppid());
	dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	if (WIFEXITED(status))
		g_ret = WEXITSTATUS(status);
}

void    builtin_echo(t_cmd *cmd)
{
	int		flag;
	int		i;

	flag = 0;
	i = 0;
	if (cmd->arg[1] && !ft_strcmp(cmd->arg[1], "-n"))
	{
		flag = 1;
		i++;
	}
	while (cmd->arg[++i])
	{
		if (cmd->append > -1 || cmd->redout > -1)
		{
			write(cmd->append + cmd->redout + 1, cmd->arg[i], (int)ft_strlen(cmd->arg[i]));
			write(cmd->append + cmd->redout + 1, " ", 1);
		}
		else
			printf("%s ", cmd->arg[i]);
	}
	if (!flag)
	{
		if (cmd->append > -1 || cmd->redout > -1)
			write(cmd->append + cmd->redout + 1, "\n", 1);
		else
			printf("\n");
	}
	if (cmd->append > -1 || cmd->redout > -1)
		close(cmd->append + cmd->redout + 1);
	g_ret = 0;
}

void    builtin_export(t_cmd *cmd, t_list **envl)
{
	int		i;
	char	*key;
	char	*val;

	if (!cmd->arg[1])
	{
		builtin_env(*envl, 1);
		return ;
	}
	i = 0;
	while (cmd->arg[++i])
	{
		key = parse_key(cmd->arg[i]);
		val = parse_value(cmd->arg[i]);
		if (key && val)
		{
			ft_lstadd_last(envl, ft_lstnew(key, val));
			g_ret = 0;
		}
		else
		{
			if (cmd->arg[i][0] == ' ' || cmd->arg[i][0] == '=')
			{
				printf("minishell: %s: '%s': not a valid identifier\n", cmd->arg[0], cmd->arg[i]);
				g_ret = 1;
			}
			if (key)
				free(key);
			if (val)
				free(val);
		}
	}
}

void    builtin_unset(t_cmd *cmd, t_list **envl)
{
	int		i;

	i = 0;
	while (cmd->arg[++i])
		ft_lstdel_key(envl, cmd->arg[i]);
	g_ret = 0;
}

void    builtin_env(t_list *envl, int flag)
{
	t_list	*tmp;

	tmp = envl;
	while (tmp)
	{
		if (flag)
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		else
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	g_ret = 0;
}

void    builtin_cd(char *dir, t_list **envl)
{
	int		status;

	ft_lstadd_last(envl, ft_lstnew("OLDPWD", getcwd(NULL, 0)));
	status = chdir(dir);
	if (status < 0)
	{
		printf("minishell: cd: %s: %s\n", dir, strerror(errno));
		g_ret = 1;
	}
	else
		g_ret = 0;
}
