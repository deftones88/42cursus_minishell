#include "minishell.h"

void	init_cmd(t_cmd *cmd)
{
	cmd->arg = 0;
	cmd->flag = 0;
	cmd->redin = 0;
	cmd->redout = 0;
	cmd->append = 0;
	cmd->delimit = 0;
	cmd->env.env_ret = 0;
}

char	**init_env(char **envp)
{
	char	**tmp;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	tmp = ft_calloc(sizeof(char *), i + 1);
	if (!tmp)
	{
		printf("Malloc error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	i = -1;
	while (envp[++i])
		tmp[i] = ft_strdup(envp[i]);
	return (tmp);
}
