/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:14:54 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/05 15:14:57 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_cmd(t_cmd **first, t_cmd *new)
{
	t_cmd	*cur;

	if (!*first)
		*first = new;
	else
	{
		cur = *first;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
	new->next = 0;
}

t_cmd	*init_cmd(char *line, t_list *envl, int total)
{
	int		i[2];
	char	quot;
	char	flag;
	t_cmd	*head;
	t_cmd	*cur;

	i[0] = 0;
	i[1]= 0;
	quot = 0;
	head = 0;
	while (1)
	{
		if (!quot && (line[i[0]] == '\'' || line[i[0]] == '\"'))
			quot = line[i[0]];
		else if (quot && line[i[0]] == quot)
			quot = 0;
		// if (!quot && (line[i] == ';' || line[i] == '|' || line[i] == 0))
		if (!quot && (line[i[0]] == 0))
		{
			flag = line[i[0]];
			line[i[0]] = 0;
			cur = (t_cmd*)ft_calloc(1, sizeof(t_cmd));
			cur->flag = total;
			parse_tmp(line + i[1], cur, envl);
			add_cmd(&head, cur);
			i[1]= i[0] + 1;
			line[i[0]] = flag;
		}
		if (line[i[0]] == 0)
			break ;
		i[0]++;
	}
	if (quot)
	{
		printf("minishell: '%c': syntax error\n", quot);
		return (0);
	}
	printf("\e[32m-- init cmd\n\e[0m");
	return (head);
}
