/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:14:16 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/18 16:43:50 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	ft_echo(t_cmd *cmd, int i, int flag)
{
	while (cmd->arg[++i])
	{
		if (cmd->append > -1 || cmd->redout > -1)
		{
			if (i > flag + 1)
				write(cmd->append + cmd->redout + 1, " ", 1);
			write(cmd->append + cmd->redout + 1,
				cmd->arg[i], (int)ft_strlen(cmd->arg[i]));
		}
		else
		{
			if (i > flag + 1)
				printf(" ");
			printf("%s", cmd->arg[i]);
		}
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
}

void	ft_export(t_cmd *cmd, int i, char *key, char *val)
{
	printf("minishell: %s: '%s': not a valid identifier\n",
		cmd->arg[0], cmd->arg[i]);
	g_ret = 1;
	if (key)
		free(key);
	if (val)
		free(val);
}
