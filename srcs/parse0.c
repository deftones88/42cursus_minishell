/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:23:27 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/20 16:41:58 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_ret;

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
			val = find_value(p->envl, key);
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

void	parse_var_3(t_parse *p, char *buf)
{
	char	*val;

	val = find_value(p->envl, "HOME");
	p->k = -1;
	while (val[++p->k])
		buf[p->i++] = val[p->k];
	free(val);
	p->j += 1;
}

void	parse_var(char *buf, char *line, t_list *envl)
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
		if (!p->quot && line[p->j] == '~' && (line[p->j + 1] == 0
			|| line[p->j + 1] ==  ' ' || line[p->j + 1] == '/'))
			parse_var_3(p, buf);
		if (p->quot != '\'' && line[p->j] == '$')
			red = parse_var_0(p, buf, line, red);
		else
			buf[p->i++] = line[p->j++];
	}
	buf[p->i] = 0;
	free(p);
}
