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

void	init_pid(t_all *all, t_pid *pid, char *line)
{
	int		i;

	pid->pipe_cmd = ft_split(line, "|");
	cat_check(all, pid);
	i = 0;
	while (pid->pipe_cmd[all->idx + i])
	{
		pid->pipe_cmd[i] = pid->pipe_cmd[all->idx + i];
		i++;
	}
	pid->pipe_cmd[i] = 0;
	pid->total -= all->idx;
	pid->pid = malloc(sizeof(pid_t) * pid->total);
	if (!pid->pid)
		err_msg("malloc failed\n");
}

t_all	*init_all(char **envp)
{
	t_all	*tmp;

	show_logo();
	signal(SIGQUIT, SIG_IGN);
	tmp = ft_calloc(1, sizeof(t_all));
	if (!tmp)
		err_msg("malloc failed\n");
	init_fd(&tmp->fd);
	tcgetattr(STDIN_FILENO, &tmp->t_old);
	set_termios(0);
	tmp->envl = init_env(envp);
	return (tmp);
}
