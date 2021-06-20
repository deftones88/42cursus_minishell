#ifndef MINISHELL_H
# define MINISHELL_H

# include "get_next_line.h"
# include "libft.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
// # include <list.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>

typedef struct	s_env
{
	int		is_env;				// env flag;
	char	*env_str;			// env string;
	int		len;					// env strlen;
	int		single;				// single quote flag;
}			t_env;

typedef struct	s_cmd
{
	char	*cmd;					// command
	char	**arg;				// command + options
	int		flag;					//
	char	*redin;				// <
	char	*redout;			// >
	char	*append;			// >>
	char	*delimit;			// <<
	t_env	env;					// env struct
}				t_cmd;

/*
** init.c
*/
void init_cmd(t_cmd *cmd);

/*
** parse.c
*/
//
void cmd_print(t_cmd *cmd);
//
void free_cmd(t_cmd *cmd);
char **split_line(char *line, t_cmd *cmd);
void parse_tmp(char *line, t_cmd *cmd);

/*
** parse_check.c
*/
int  check_builtin(char *arg);
int  check_closing_quotation(char *line, char c, int *a);
int  check_env(char *line, t_cmd *cmd, int flag);

#endif
