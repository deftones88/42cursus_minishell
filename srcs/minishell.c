/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:16:37 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/18 16:51:05 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_ret = 0;

int	quot_check(char *line)
{
	int		i;
	char	quot;

	quot = 0;
	i = -1;
	while (line[++i])
	{
		if (!quot && (line[i] == '\'' || line[i] == '\"'))
			quot = line[i];
		else if (quot && line[i] == quot)
			quot = 0;
	}
	if (quot)
	{
		g_ret = 1;
		printf("minishell: '%c': syntax error\n", quot);
		free(line);
		return (1);
	}
	return (0);
}

void	mainloop(t_all *all, char *line)
{
	int		i;

	add_history(line);
	if (check_pipe_char(line) || quot_check(line))
		continue ;
	init_pid(all, &all->pid, line);
	pid_loop(all);
	i = -1;
	while (++i < all->idx)
		free(readline(""));
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_all	*all;

	all = init_all(envp);
	while (1)
	{
		signal(SIGINT, sig_handler2);
		line = readline("minishell$ ");
		signal(SIGINT, SIG_DFL);
		if (line && line[0] != 0)
			mainloop(all, line);
		else if (line == NULL)
		{
			set_termcap(0);
			printf("minishell$ exit\n");
			tcsetattr(STDIN_FILENO, TCSANOW, &all->t_old);
			exit(EXIT_SUCCESS);
		}
		free(line);
	}
	return (0);
}
