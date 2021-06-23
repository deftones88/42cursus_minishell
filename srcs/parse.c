#include "minishell.h"

void free_cmd(t_cmd *cmd)
{
	freeall(0, cmd->arg);
	if (cmd->cmd)
		free(cmd->cmd);
	if (cmd->redin)
		free(cmd->redin);
	if (cmd->redout)
		free(cmd->redout);
	if (cmd->append)
		free(cmd->append);
	if (cmd->delimit)
		free(cmd->delimit);
	if (cmd->env.env_ret)
		free(cmd->env.env_str);
}

static char	*get_key(char *line, int *i)
{
	char	buf[128];
	int		k;

	k = 0;
	if (line[*i] == '$')
		++*i;
	if (line[*i] == '{')
		++*i;
	while (ft_isalpha(line[*i]) || ft_isdigit(line[*i]) || line[*i] == '_')
		buf[k++] = line[(*i)++];
	if (line[*i] == '}')
		++*i;
	buf[k] = 0;
	return (ft_strdup(buf));
}

void	parse_var(char *buf, char *line, t_list *envl)
{
	int		i;
	int		j;
	int		k;
	char	quot;
	char	*key;
	char	*val;

	quot = 0;
	i = 0;
	j = 0;
	while (line[j])
	{
		if (!quot && (line[j] == '\'' || line[j] == '\"'))
			quot = line[j];
		else if (quot && line[j] == quot)
			quot = 0;
		if (quot != '\'' && line[j] == '$')
		{
			key = get_key(line, &j);
			val = find_value(envl, key);
			k = -1;
			while (val[++k])
				buf[i++] = val[k];
			free(key);
		}
		else
			buf[i++] = line[j++];
	}
	buf[i] = 0;
}

void	parse_tmp(char *line, t_cmd *cmd, t_list *envl)
{
	char		buf[65536];

	parse_var(buf, line, envl);
	cmd->arg = ft_split(buf, ' ');
}
