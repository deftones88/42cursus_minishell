/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:13:47 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/05 15:13:50 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

char	**list2arr(t_list *envl)
{
	t_list	*cur;
	char	**ret;
	char	*tmp;
	int		n;

	cur = envl;
	n = 0;
	while (cur)
	{
		cur = cur->next;
		n++;
	}
	ret = (char**)ft_calloc(n + 1, sizeof(char*));
	n = 0;
	cur = envl;
	while (cur)
	{
		tmp = ft_strjoin(cur->key, "=");
		ret[n] = ft_strjoin(tmp, cur->value);
		free(tmp);
		n++;
		cur = cur->next;
	}
	return (ret);
}

char	*parse_key(char *str)
{
	int		i;
	char	*buf;
	char	*key;

	i = -1;
	buf = ft_strdup(str);
	while (buf[++i] && buf[i] != '=')
		;
	if (i != 0 && buf[i] == '=' && buf[0] != ' ')
	{
		buf[i] = 0;
		key = ft_strdup(buf);
		free(buf);
		return (key);
	}
	else
	{
		free(buf);
		return (0);
	}
}

char	*parse_value(char *str)
{
	int		i;

	i = -1;
	while (str[++i] && str[i] != '=')
		;
	if (i != 0 && str[i] == '=')
		return (ft_strdup(str + i + 1));
	else
		return (0);
}

char	*find_value(t_list *envl, char *key)
{
	t_list	*cur;

	cur = envl;
	while (cur)
	{
		if (!ft_strcmp(cur->key, key))
			return (cur->value);
		cur = cur->next;
	}
	return (ft_strdup(""));
}

t_list	*init_env(char **envp)
{
	int		i;
	t_list	*root;

	i = -1;
	root = 0;
	while (envp[++i])
		ft_lstadd_last(&root,
				ft_lstnew(parse_key(envp[i]), parse_value(envp[i])));
	ft_lstadd_last(&root, ft_lstnew("OLDPWD", ""));
	return (root);
}
