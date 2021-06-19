#ifndef MINISHELL_H
# define MINISHELL_H

# include "get_next_line.h"
# include "libft.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <list.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct	s_cmd
{
	char	*cmd;					// command
	char	**arg;				// command + options
	int		flag;					//
	char	*redin;				// <
	char	*redout;			// >
	char	*append;			// >>
	char	*delimit;			// <<
}				t_cmd;

/*
** parse.c
*/
//
void cmd_print(t_cmd *cmd);
//
void init_cmd(t_cmd *cmd);
void parse_tmp(char *line, t_cmd *cmd);

#endif
