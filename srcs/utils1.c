/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:18:02 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/20 16:42:00 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe_char(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] == ' ')
		i++;
	if (arg[i] == '|')
	{
		if (arg[i + 1] == '|')
			printf("minishell: syntax error near unexpected token '||'\n");
		else
			printf("minishell: syntax error near unexpected token '|'\n");
		return (1);
	}
	i = ft_strlen(arg);
	while (--i > 0 && arg[i] && arg[i] == ' ')
		;
	if (arg[i] == '|')
	{
		printf("minishell: syntax error near unexpected token '|'\n");
		return (1);
	}
	return (0);
}

t_cmd	*free_next(t_cmd *cmd)
{
	t_cmd	*next;

	next = cmd->next;
	free_all(cmd->arg);
	if (cmd->cmd)
		free(cmd->cmd);
	free(cmd);
	return (next);
}
