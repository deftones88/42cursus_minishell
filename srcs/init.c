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

t_cmd	*init_cmd(char *line, t_list *envl)
{
	t_cmd	*head;

	head = 0;
	head = (t_cmd*)ft_calloc(1, sizeof(t_cmd));
	if (parse(line, head, envl))
		return (0);
	return (head);
}

void	init_fd(t_fd *fd)
{
	fd->fd_bu[0] = dup(STDIN_FILENO);
	fd->fd_bu[1] = dup(STDOUT_FILENO);
}

void	init_pid(t_pid *pid, char *line)
{
	pid->pipe_cmd = ft_split(line, "|");
	pid->total = 0;
	while (pid->pipe_cmd[pid->total])
		pid->total++;
	pid->pid = malloc(sizeof(pid_t) * pid->total);
	if (!pid->pid)
		err_msg("malloc failed\n");
}

t_all	*init_all(char **envp)
{
	t_all	*tmp;

	show_logo();
	tmp = ft_calloc(1, sizeof(t_all));
	if (!tmp)
		err_msg("malloc failed\n");
	init_fd(&tmp->fd);
	tcgetattr(STDIN_FILENO, &tmp->t_old);
	set_termios(0);
	tmp->envl = init_env(envp);
	return (tmp);
}
