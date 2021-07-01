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

void	ft_exec(t_cmd *cmd, t_list *envl)
{
	pid_t	pid;
	char	**envp;
	int		status;

	envp = list2arr(envl);
	merge_path(cmd, find_value(envl, "PATH"));
	pid = fork();
	if (pid == 0)
	{
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
	wait(&status);
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
		builtin_env(cmd, *envl, 1);
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

void    builtin_env(t_cmd *cmd, t_list *envl, int flag)
{
	t_list	*tmp;

	tmp = envl;
	while (tmp)
	{
		if (flag)
		{
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		}
		else
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	g_ret = 0;
}
