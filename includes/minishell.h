/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:11:50 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/05 15:11:53 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "struct.h"
# include "get_next_line.h"
# include "libft.h"
# include "utils.h"
# include "sig_handler.h"
# include "builtin.h"
# include "list.h"
# include "env.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <limits.h>

# ifndef ARG_MAX
#  define ARG_MAX 2048
# endif

enum	e_CMD_exit_code
{
	CMD_EXIT = 1,
	CMD_CD,
	CMD_RED
};

/*
** init.c
*/
void	add_cmd(t_cmd **first, t_cmd *new);
t_cmd	*init_cmd(char *line, t_list *envl);
void	init_fd(t_fd *fd);
void	init_pid(t_pid *pid, char *line);
t_all	*init_all(char **envp);

/*
** parse.c
*/
t_cmd	*free_next(t_cmd *cmd);
char	**split_line(char *line, t_cmd *cmd, t_list *envl);
void	parse_tmp(char *line, t_cmd *cmd, t_list *envl);

/*
** redirection.c
*/
void	heredoc_child(int fd[2], char *delimiter);
int		heredoc_parent(t_cmd *cmd, char *buf, int fd[2], int flag);
int		heredoc_eof(char *buf, char *delimiter);
int		heredoc_all(t_cmd *cmd, char *buf, int i);
int		redin(t_cmd *cmd);
int		redout_append(t_cmd *cmd, int *this, int *other, int flag);

/*
** pipe.c
*/
void	dup_close(int fd, int dup);
void	set_fd(t_fd *fd, int total, int i);
void	exit_status(t_all *all, int status);
void	cd_pipe(int fd, char *arg);

/*
** exec.c
*/
void	cmd_loop(t_all *all);
int	pid_child_loop(t_all *all, int i);
void	pid_parent_loop(t_all *all, int i);
void	pid_loop(t_all *all);

#endif
