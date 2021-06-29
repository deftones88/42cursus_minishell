#include "minishell.h"

t_cmd	*free_next(t_cmd *cmd)
{
	t_cmd	*next;

	next = cmd->next;
	free_all(cmd->arg);
	if (cmd->cmd)
		free(cmd->cmd);
	if (cmd->delimit)
		free(cmd->delimit);
	return (next);
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
	if (quot)
		err_msg("parse error:quot");
	buf[i] = 0;
}

void	parse_red(char *buf, t_cmd *cmd)
{
	int		i;
	char	quot;
	char	**tmp;

	quot = 0;
	i = -1;
	while (buf[++i])
	{
		if (!quot && (buf[i] == '\'' || buf[i] == '\"'))
			quot = buf[i];
		else if (quot && buf[i] == quot)
			quot = 0;
		if (!quot && (buf[i] == '<' || buf[i] == '>'))
		{
			if (buf[i + 1] == buf[i])
				tmp = ft_split(buf + i + 2, " <>");
			else
				tmp = ft_split(buf + i + 1, " <>");
			if (!tmp[0])
				err_msg("parse_error!\n");
			if (buf[i] == '<')
			{
				if (buf[i + 1] == '<')
				{
					//heredoc exit string
					cmd->delimit = ft_strdup(tmp[0]);
					//execute right away
					free(cmd->delimit);
				}
				else
				{
					if (cmd->redin > -1)
						close(cmd->redin);
					cmd->redin = open(tmp[0], O_RDONLY, 0);
					if (cmd->redin < 0)
					{
						printf("minishell: %s: %s\n", tmp[0], strerror(errno));
						cmd->ret = errno;
						free_all(tmp);
						return ;
					}
				}
			}
			else
			{
				if (buf[i + 1] == '>')
				{
					if (cmd->append > -1)
						close(cmd->append);
					cmd->append = open(tmp[0], O_CREAT | O_APPEND | O_WRONLY, 0644);
					if (cmd->redout > -1)
					{
						close(cmd->redout);
						cmd->redout = -1;
					}
				}
				else
				{
					if (cmd->redout > -1)
						close(cmd->redout);
					cmd->redout = open(tmp[0], O_CREAT | O_TRUNC | O_WRONLY, 0644);
					if (cmd->append > -1)
					{
						close(cmd->append);
						cmd->append = -1;
					}
				}
			}
			if (buf[i + 1] == buf[i])
				buf[i + 1] = ' ';
			buf[i++] = ' ';
			while (buf[i] == ' ')
				i++;
			while (buf[i] && (quot || buf[i] != ' '))
			{
				if (!quot && (buf[i] == '\'' || buf[i] == '\"'))
					quot = buf[i];
				else if (quot && buf[i] == quot)
					quot = 0;
				if (!quot && (buf[i] == '<' || buf[i] == '>'))
					err_msg("parse_error!\n");
				buf[i++] = ' ';
			}
			i--;
			free_all(tmp);
		}
	}
}

void	parse_tmp(char *line, t_cmd *cmd, t_list *envl)
{
	char		buf[10000];

	cmd->redin = -1;
	cmd->redout = -1;
	cmd->append = -1;
	parse_var(buf, line, envl);
	parse_red(buf, cmd);
	//printf("\n%s\n%s\n%s\n%s\n%s\n",buf,cmd->redin, cmd->redout, cmd->append, cmd->delimit);
	cmd->arg = ft_split(buf, " ");
}
