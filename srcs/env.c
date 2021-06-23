#include "env.h"

char	*parse_key(char *str)
{
	int		i;
	char	*buf;
	char	*key;

	i = -1;
	buf = ft_strdup(str);
	while (buf[++i] && buf[i] != '=')
		;
	if (buf[i] == '=')
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
	if (str[i] == '=')
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
	return (root);
}

