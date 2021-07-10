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
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <limits.h>

# ifndef ARG_MAX
#  define ARG_MAX 2048
# endif

# ifndef PRINT
#  define PRINT 0
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

#endif
