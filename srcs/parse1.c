/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:23:27 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/20 16:41:59 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_ret;

int	parse_red_1(t_parse *p, char *buf, t_cmd *cmd)
{
	if (buf[p->i] == '<' && buf[p->i + 1] != '>')
	{
		if (buf[p->i + 1] == '<'
			&& (buf[p->i + 2] != '<' && buf[p->i + 2] != '>'))
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
			if (!(buf[p->i] == '>' && buf[p->i + 1] == '<')
				&& (buf[p->i + 1] == '<' || buf[p->i + 1] == '>'))
				printf("minishell: syntax error near unexpected token '%c%c'\n",
					buf[p->i], buf[p->i + 1]);
			else
				printf("minishell: syntax error near unexpected token '%c'\n",
					buf[p->i]);
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
	if (buf[p->i + 1] == buf[p->i])
		buf[p->i + 1] = ' ';
	buf[p->i++] = ' ';
	if (parse_red_2(p, buf, cmd))
		return (1);
	free_all(cmd->parse);
	return (0);
}

int	parse_red(char *buf, t_cmd *cmd)
{
	t_parse	*p;
	int		ret;

	p = (t_parse *)ft_calloc(1, sizeof(t_parse));
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
	parse_var(buf, line, envl);
	if (parse_red(buf, cmd))
		return (1);
	cmd->arg = ft_strap(ft_split(buf, " "));
	return (0);
}
