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
	int		flag;

	k = 0;
	flag = 0;
	if (line[*i] == '$')
		++*i;
	if (line[*i] == '{')
	{
		++*i;
		flag = 1;
	}
	while (ft_isalpha(line[*i]) || ft_isdigit(line[*i]) || line[*i] == '_')
		buf[k++] = line[(*i)++];
	if (flag && line[*i] == '}')
	{
		++*i;
		flag--;
	}
	if (flag)
		return (0);
	buf[k] = 0;
	return (ft_strdup(buf));
}

char	*set_val(t_parse *p, char *line)
{
	char	*key;
	char	*val;

	if (line[p->j + 1] == '?')
	{
		val = ft_itoa(g_ret);
		p->j += 2;
	}
	else if (line[p->j + 1] == '_'
		|| ft_isalnum(line[p->j + 1]) || line[p->j + 1] == '{')
	{
		key = get_key(line, &p->j);
		if (!key)
			val = 0;
		else
			val = find_value(p->envl, key);
		free(key);
	}
	else if (!p->quot && (line[p->j + 1] == '\'' || line[p->j + 1] == '\"'))
	{
		val = ft_strdup("");
		p->j++;
	}
	else
	{
		val = ft_strdup("$");
		p->j++;
	}
	return (val);
}

int	parse_var_0(t_parse *p, char *buf, char *line, int *red)
{
	char	*val;

	if (!*red)
	{
		val = set_val(p, line);
		p->k = -1;
		if (!val)
			return (1);
		while (val[++p->k])
			buf[p->i++] = val[p->k];
		free(val);
	}
	else
		buf[p->i++] = line[p->j++];
	*red = 0;
	return (0);
}

int	parse_var_3(t_parse *p, char *buf, char *line, int *red)
{
	char	*val;

	if (!p->quot && line[p->j] == '~' && (line[p->j + 1] == 0
			|| line[p->j + 1] == ' ' || line[p->j + 1] == '/'))
	{
		val = find_value(p->envl, "HOME");
		p->k = -1;
		while (val[++p->k])
			buf[p->i++] = val[p->k];
		free(val);
		p->j += 1;
	}
	if (p->quot != '\'' && line[p->j] == '$')
	{
		if (parse_var_0(p, buf, line, red))
		{
			printf("minishell: syntax error, missing '}'\n");
			buf[0] = 0;
			return (1);
		}
	}
	else
		buf[p->i++] = line[p->j++];
	return (0);
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
		if (parse_var_3(p, buf, line, &red))
			break ;
	}
	buf[p->i] = 0;
	free(p);
}
