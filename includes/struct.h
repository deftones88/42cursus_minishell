/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:23:06 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/05 15:23:08 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include <sys/types.h>
# include <termios.h>
# include "list.h"

typedef struct		s_cmd
{
	char			*cmd;
	char			**arg;				// command + options
	int				flag;					// represent |(1) or ;(2)
	int				redin;				// <
	int				redout;				// >
	int				append;				// >>
	int				delimit;			// <<
	char			**parse;			// tmp from parse_tmp
	int				ret;					// skip executing cmd
	struct s_cmd	*next;
}					t_cmd;

typedef struct	s_fd
{
	int				prev_fd;
	int				fd_bu[2];
	int				fd[2];
}					t_fd;

typedef struct	s_pid
{
	pid_t		*pid;
	int			total;
	char		**pipe_cmd;
}					t_pid;

typedef struct	s_all
{
	t_pid	pid;
	t_fd	fd;
	t_list	*envl;
	t_cmd	*cmd;
	struct termios	t_old;
}	t_all;

#endif
