/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:23:27 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/05 15:24:02 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_ret;

t_cmd		*free_next(t_cmd *cmd)
{
	t_cmd	*next;

	next = cmd->next;
	free_all(cmd->arg);
	if (cmd->cmd)
		free(cmd->cmd);
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

void		parse_var(char *buf, char *line, t_list *envl, t_cmd *cmd)
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
			if (line[j + 1] == '?')
			{
				val = ft_itoa(g_ret);
				j += 2;
			}
			else
			{
				key = get_key(line, &j);
				val = find_value(envl, key);
				free(key);
			}
			k = -1;
			while (val[++k])
				buf[i++] = val[k];
		}
		else
			buf[i++] = line[j++];
	}
	if (quot)
	{
		printf("parse error: quot");
		g_ret = 1;
		cmd->ret = 1;
	}
	buf[i] = 0;
}

void		parse_red(char *buf, t_cmd *cmd)
{
	int		i;
	char	quot;

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
				cmd->parse = ft_split(buf + i + 2, " <>");
			else
				cmd->parse = ft_split(buf + i + 1, " <>");
			if (!cmd->parse[0])
			{
				printf("minishell: syntax error near unexpected token 'newline'\n");
				cmd->ret = 1;
				g_ret = 258;
				return ;
			}
			if (buf[i] == '<')
			{
				if (buf[i + 1] == '<')
				{
					if (heredoc_all(cmd, buf, i))
						continue ;
				}
				else
				{
					if (redin(cmd))
						return ;
				}
			}
			else
			{
				if (buf[i + 1] == '>')
				{
					if (redout_append(cmd, &cmd->append, &cmd->redout, O_APPEND))
						continue ;
				}
				else
				{
					if (redout_append(cmd, &cmd->redout, &cmd->append, O_TRUNC))
						continue ;
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
				{
					printf("parse error\n");
					cmd->ret = 1;
					g_ret = 1;
				}
				buf[i++] = ' ';
			}
			i--;
			free_all(cmd->parse);
		}
	}
}

void		parse_tmp(char *line, t_cmd *cmd, t_list *envl)
{
	char		buf[10000];

	cmd->redin = -1;
	cmd->redout = -1;
	cmd->append = -1;
	cmd->delimit = -1;
	cmd->ret = 0;
	parse_var(buf, line, envl, cmd);
	parse_red(buf, cmd);
	// printf("\n%s\n%d\n%d\n%d\n%s\n",buf,cmd->redin, cmd->redout, cmd->append, cmd->delimit);
	cmd->arg = ft_split(buf, " ");
}
