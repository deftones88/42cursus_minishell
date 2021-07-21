/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:14:16 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/18 16:42:11 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	builtin_echo(t_cmd *cmd)
{
	int		flag;
	int		i;
	int		j;

	flag = 0;
	i = 0;
	while (cmd->arg[i + 1]
		&& cmd->arg[i + 1][0] == '-' && cmd->arg[i + 1][1] == 'n')
	{
		j = 2;
		while (!ft_strcmp(cmd->arg[0], "echo")
			&& cmd->arg[i + 1][j] && cmd->arg[i + 1][j] == 'n')
			j++;
		if (cmd->arg[i + 1][j] != '\0')
			break ;
		flag++;
		i++;
	}
	ft_echo(cmd, i, flag);
	g_ret = 0;
}

void	builtin_export(t_cmd *cmd, t_list **envl)
{
	int		i;
	char	*key;
	char	*val;

	if (!cmd->arg[1])
	{
		builtin_env(*envl, 1);
		return ;
	}
	i = 0;
	while (cmd->arg[++i])
	{
		key = parse_key(cmd->arg[i]);
		val = parse_value(cmd->arg[i]);
		if (key && val)
		{
			ft_lstadd_last(envl, ft_lstnew(key, val));
			g_ret = 0;
		}
		else
			ft_export(cmd, i, key, val);
	}
}

void	builtin_unset(t_cmd *cmd, t_list **envl)
{
	int		i;
	int		j;
	int		flag;

	i = 0;
	while (cmd->arg[++i])
	{
		j = -1;
		flag = ft_isalpha(cmd->arg[i][0]) || cmd->arg[i][0] == '_';
		while (flag && cmd->arg[i][++j])
			if (!ft_isalnum(cmd->arg[i][j]) && cmd->arg[i][j] != '_')
				flag = 0;
		if (flag)
			ft_lstdel_key(envl, cmd->arg[i]);
		else
			printf("minishell: %s: '%s': not a valid identifier\n",
				cmd->arg[0], cmd->arg[i]);
	}
	g_ret = 0;
}

void	builtin_env(t_list *envl, int flag)
{
	t_list	*tmp;

	tmp = envl;
	while (tmp)
	{
		if (flag)
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		else
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	g_ret = 0;
}

void	builtin_cd(char *dir, t_list **envl)
{
	int		status;
	char	*home;

	home = getcwd(NULL, 0);
	ft_lstadd_last(envl, ft_lstnew(ft_strdup("OLDPWD"), home));
	if (!dir)
	{
		home = find_value(*envl, "HOME");
		status = chdir(home);
		free(home);
	}
	else
		status = chdir(dir);
	if (status < 0)
	{
		printf("minishell: cd: %s: %s\n", dir, strerror(errno));
		g_ret = 1;
	}
	else
		g_ret = 0;
}
