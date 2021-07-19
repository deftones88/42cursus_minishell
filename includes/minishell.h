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
# include <unistd.h>
# ifndef ARG_MAX
#  define ARG_MAX 2048
# endif

enum	e_CMD_exit_code
{
	CMD_EXIT = 2,
	CMD_CD,
	CMD_EXPT,
	CMD_UNST,
	CMD_SYN
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
int		parse(char *line, t_cmd *cmd, t_list *envl);

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
void	send_thru_pipe(t_all *all, char **arg, int flag);
void	exit_status(t_all *all, int status);

/*
** loop.c
*/
void	cmd_loop(t_all *all);
void	pid_child_loop(t_all *all, int i);
void	pid_parent_loop(t_all *all, int i);
void	pid_loop(t_all *all);

/*
** ft_exec.c
*/
void	merge_path(t_cmd *cmd, char *str);
void	ft_exec_child(t_cmd *cmd, char **envp);
void	ft_exec(t_cmd *cmd, t_list *envl);

/*
** logo.c
*/
void	show_logo(void);

#endif
