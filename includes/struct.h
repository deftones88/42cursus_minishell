/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:23:06 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/20 16:21:41 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include <sys/types.h>
# include <termios.h>
# include "list.h"

typedef struct termios	t_term;

typedef struct s_cmd
{
	char			*cmd;
	char			**arg;
	int				flag;
	int				redin;
	int				redout;
	int				append;
	int				delimit;
	char			**parse;
	int				ret;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_fd
{
	int				prev_fd;
	int				fd_bu[2];
	int				fd[2];
}					t_fd;

typedef struct s_pid
{
	int			total;
	char		**pipe_cmd;
	pid_t		*pid;
}					t_pid;

typedef struct s_all
{
	int		idx;
	t_pid	pid;
	t_fd	fd;
	t_list	*envl;
	t_cmd	*cmd;
	t_term	t_old;
}	t_all;

typedef struct	s_parse
{
	int		i;
	int		j;
	int		k;
	char	quot;
	t_list	*envl;
}				t_parse;

#endif
