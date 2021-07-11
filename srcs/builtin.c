/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:14:16 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/05 15:14:18 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

extern int	g_ret;

void    builtin_echo(t_cmd *cmd)
{
	int		flag;
	int		i;
	int		j;

	flag = 0;
	i = 0;
	/*
	**  echo -nnnnnnnnnn hi      : flag 1, prints "hi$"
	**  Echo -nnnnnnnnnn hi      : flag 0, prints "-nnnnnnnnnn hi\n"
	**	Echo -n hi							 : flag 1, prints "hi$"
	**	echo ----n hi						 : flag 0, prints "----n hi$"
	*/
	while (cmd->arg[i + 1] && cmd->arg[i + 1][0] == '-' && cmd->arg[i + 1][1] == 'n')
	{
		j = 2;
		while (!ft_strcmp(cmd->arg[0], "echo") && cmd->arg[i + 1][j] && cmd->arg[i + 1][j] == 'n')
			j++;
		if (cmd->arg[i + 1][j] != '\0')
			break ;
		flag = 1;
		i++;
	}
	while (cmd->arg[++i])
	{
		if (cmd->append > -1 || cmd->redout > -1)
		{
			write(cmd->append + cmd->redout + 1, cmd->arg[i], (int)ft_strlen(cmd->arg[i]));
			write(cmd->append + cmd->redout + 1, " ", 1);
		}
		else
			printf("%s ", cmd->arg[i]);
	}
	if (!flag)
	{
		if (cmd->append > -1 || cmd->redout > -1)
			write(cmd->append + cmd->redout + 1, "\n", 1);
		else
			printf("\n");
	}
	if (cmd->append > -1 || cmd->redout > -1)
		close(cmd->append + cmd->redout + 1);
	g_ret = 0;
}

void    builtin_export(t_cmd *cmd, t_list **envl)
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
		{
			if (cmd->arg[i][0] == ' ' || cmd->arg[i][0] == '=')
			{
				printf("minishell: %s: '%s': not a valid identifier\n", cmd->arg[0], cmd->arg[i]);
				g_ret = 1;
			}
			if (key)
				free(key);
			if (val)
				free(val);
		}
	}
}

void    builtin_unset(t_cmd *cmd, t_list **envl)
{
	int		i;

	i = 0;
	while (cmd->arg[++i])
		ft_lstdel_key(envl, cmd->arg[i]);
	g_ret = 0;
}

void    builtin_env(t_list *envl, int flag)
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

void    builtin_cd(char *dir, t_list **envl)
{
	int		status;

	ft_lstadd_last(envl, ft_lstnew("OLDPWD", getcwd(NULL, 0)));
	status = chdir(dir);
	if (status < 0)
	{
		printf("minishell: cd: %s: %s\n", dir, strerror(errno));
		g_ret = 1;
	}
	else
		g_ret = 0;
}
