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

t_cmd	*init_cmd(char *line, t_list *envl)
{
	int		i;
	int		j;
	char	quot;
	char	flag;
	t_cmd	*head;
	t_cmd	*cur;

	i = 0;
	j = 0;
	quot = 0;
	head = 0;
	while (1)
	{
		if (!quot && (line[i] == '\'' || line[i] == '\"'))
			quot = line[i];
		else if (quot && line[i] == quot)
			quot = 0;
		if (!quot && (line[i] == ';' || line[i] == '|' || line[i] == 0))
		{
			flag = line[i];
			line[i] = 0;
			cur = (t_cmd*)ft_calloc(1, sizeof(t_cmd));
			parse_tmp(line + j, cur, envl);
			add_cmd(&head, cur);
			j = i + 1;
			line[i] = flag;
		}
		if (line[i] == 0)
			break ;
		i++;
	}
	if (quot)
	{
		printf("minishell: '%c': syntax error\n", quot);
		return (0);
	}
	return (head);
}
