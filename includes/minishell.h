#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct		s_cmd
{
	char			*cmd;
	char			**arg;				// command + options
	int				flag;					// represent |(1) or ;(2)
	int				redin;				// <
	int				redout;				// >
	int				append;				// >>
	int				delimit;			// <<
	int				ret;					// skip executing cmd
	struct s_cmd	*next;
}					t_cmd;

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

#endif
