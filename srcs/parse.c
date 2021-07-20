/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:23:27 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/20 16:23:46 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_ret;

t_cmd	*free_next(t_cmd *cmd)
{
	t_cmd	*next;

	next = cmd->next;
	free_all(cmd->arg);
	if (cmd->cmd)
		free(cmd->cmd);
	free(cmd);
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
int	parse_var_0(t_parse *p, char *buf, char *line, int red)
{
	char	*key;
	char	*val;

	if (!red)
	{
		if (line[p->j + 1] == '?')
		{
			val = ft_itoa(g_ret);
			p->j += 2;
		}
		else
		{
			key = get_key(line, &p->j);
			val = ft_strdup(find_value(p->envl, key));
			free(key);
		}
		p->k = -1;
		while (val[++p->k])
			buf[p->i++] = val[p->k];
		free(val);
	}
	else
		buf[p->i++] = line[p->j++];
	return (0);
}

void	parse_var(char *buf, char *line, t_list *envl, t_cmd *cmd)
{
	int		red;
	t_parse	*p;

	p = (t_parse *)ft_calloc(1, sizeof(t_parse));
	p->envl = envl;
	red = 0;
	while (line[p->j])
	{
		if (!p->quot && (line[p->j] == '\'' || line[p->j] == '\"'))
			p->quot = line[p->j];
		else if (p->quot && line[p->j] == p->quot)
			p->quot = 0;
		if (line[p->j] == '<' || line[p->j] == '>')
			red = 1;
		if (p->quot != '\'' && line[p->j] == '$')
			red = parse_var_0(p, buf, line, red);
		else
			buf[p->i++] = line[p->j++];
	}
	buf[p->i] = 0;
}

int	parse_red_1(t_parse *p, char *buf, t_cmd *cmd)
{
	if (buf[p->i] == '<' && buf[p->i + 1] != '>')
	{
		if (buf[p->i + 1] == '<' && (buf[p->i + 2] != '<' && buf[p->i + 2] != '>'))
		{
			if (heredoc_all(cmd, buf, p->i + 2))
				return (2);
		}
		else if (buf[p->i + 1] != '<')
		{
			if (redin(cmd))
				return (1);
		}
	}
	else if (buf[p->i + 1] == '>')
	{
		if (redout_append(cmd, &cmd->append, &cmd->redout, O_APPEND))
			return (2);
	}
	else if (redout_append(cmd, &cmd->redout, &cmd->append, O_TRUNC))
		return (2);
	return (0);
}

int	parse_red_2(t_parse *p, char *buf, t_cmd *cmd)
{
	if (buf[p->i + 1] == buf[p->i])
		buf[p->i + 1] = ' ';
	buf[p->i++] = ' ';
	while (buf[p->i] == ' ')
		p->i++;
	while (buf[p->i] && (p->quot || buf[p->i] != ' '))
	{
		if (!p->quot && (buf[p->i] == '\'' || buf[p->i] == '\"'))
			p->quot = buf[p->i];
		else if (p->quot && buf[p->i] == p->quot)
			p->quot = 0;
		if (!p->quot && (buf[p->i] == '<' || buf[p->i] == '>'))
		{
			if (!(buf[p->i] == '>' && buf[p->i + 1] == '<') && (buf[p->i + 1] == '<' || buf[p->i + 1] == '>'))
				printf("minishell: syntax error near unexpected token '%c%c'\n", buf[p->i], buf[p->i + 1]);
			else
				printf("minishell: syntax error near unexpected token '%c'\n", buf[p->i]);
			g_ret = 258;
			cmd->ret = 1;
			return (1);
		}
		buf[p->i++] = ' ';
	}
	p->i--;
	return (0);
}

int	parse_red_0(t_parse *p, char *buf, t_cmd *cmd)
{
	int	ret;

	if (buf[p->i + 1] == buf[p->i])
		cmd->parse = ft_split(buf + p->i + 2, " <>");
	else
		cmd->parse = ft_split(buf + p->i + 1, " <>");
	if (!cmd->parse[0])
	{
		printf("minishell: syntax error near unexpected token 'newline'\n");
		cmd->ret = 1;
		g_ret = 258;
		return (1);
	}
	ret = parse_red_1(p, buf, cmd);
	if (ret == 1)
		return (1);
	else if (ret == 2)
		return (2);
	if (parse_red_2(p, buf, cmd))
		return (1);
	free_all(cmd->parse);
	return (0);
}

int	parse_red(char *buf, t_cmd *cmd)
{
	t_parse	*p;
	int	ret;

	p = (t_parse*)ft_calloc(1, sizeof(t_parse));
	p->i = -1;
	while (buf[++p->i])
	{
		if (!p->quot && (buf[p->i] == '\'' || buf[p->i] == '\"'))
			p->quot = buf[p->i];
		else if (p->quot && buf[p->i] == p->quot)
			p->quot = 0;
		if (!p->quot && (buf[p->i] == '<' || buf[p->i] == '>'))
		{
			ret = parse_red_0(p, buf, cmd);
			if (ret == 1)
				return (1);
			else if (ret == 2)
				continue ;
		}
	}
	free(p);
	return (0);
}

int	parse(char *line, t_cmd *cmd, t_list *envl)
{
	char		buf[10000];

	cmd->redin = -1;
	cmd->redout = -1;
	cmd->append = -1;
	cmd->delimit = -1;
	cmd->ret = 0;
	ft_bzero(buf, 10000);
	parse_var(buf, line, envl, cmd);
	if (parse_red(buf, cmd))
		return (1);
	cmd->arg = ft_strap(ft_split(buf, " "));
	return (0);
}
