#include "builtin.h"

void	free_all(char **strs)
{
	int		i;

	i = -1;
	while (strs[++i])
		free(strs[i]);
	free(strs);
}

void	merge_path(t_cmd *cmd, char *str)
{
	char		**path;
	int			i;
	char		*tmp[2];
	struct stat	s;

	path = ft_split(str, ':');
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
	
	envp = list2arr(envl);
	merge_path(cmd, find_value(envl, "PATH"));
	pid = fork();
	if (pid == 0)
	{
		if (cmd->cmd && execve(cmd->cmd, cmd->arg, envp) == -1)
		{
			cmd->ret = errno;
			printf("Error executing : %s\n", strerror(errno));
		}
		else
			printf("Command not found: %s\n", cmd->arg[0]);
		exit(0);
	}
	wait(0);
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
		printf("%s ", cmd->arg[i]);
	if (!flag)
		printf("\n");
	cmd->ret = 0;
}

void    builtin_export(t_cmd *cmd, t_list **envl)
{
	int		i;
	char	*key;
	char	*val;

	i = 0;
	while (cmd->arg[++i])
	{
		key = parse_key(cmd->arg[i]);
		val = parse_value(cmd->arg[i]);
		if (key && val)
			ft_lstadd_last(envl, ft_lstnew(key, val));
		else
		{
			printf("%s: '%s': not a valid identifier\n", cmd->arg[0], cmd->arg[i]);
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
}

void    builtin_env(t_cmd *cmd, t_list *envl)
{
	t_list	*tmp;

	tmp = envl;
	while (tmp)
	{
		printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	cmd->ret = 0;
}
