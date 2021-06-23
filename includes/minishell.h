#ifndef MINISHELL_H
# define MINISHELL_H
# ifndef ARG_MAX
#  define ARG_MAX 2048
# endif
# include "get_next_line.h"
# include "libft.h"
# include "utils.h"
# include "sig_handler.h"


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
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
// # include <dirent.h>

typedef struct	s_env
{
	int		is_env;				// env flag
	char	*env_str;			// env string
	int		len;					// env strlen
	int		single;				// single quote flag
	int		env_ret;			// strig malloc flag
}			t_env;

typedef struct	s_cmd
{
	char	**arg;				// command + options
	int		flag;					//
	char	*redin;				// <
	char	*redout;			// >
	char	*append;			// >>
	char	*delimit;			// <<
	int		ret;					// return value of previous execution
											// global?
	t_env	env;					// env struct
}				t_cmd;

/*
** init.c
*/
void	init_cmd(t_cmd *cmd);

/*
** parse.c
*/
//
void	cmd_print(t_cmd *cmd);
//
void	free_cmd(t_cmd *cmd);
char	**split_line(char *line, t_cmd *cmd, t_list *envl);
void	parse_tmp(char *line, t_cmd *cmd, t_list *envl);

/*
** check_parse.c
*/
int		check_builtin(char *arg);

/*
** check_env.c
*/
char	check_env_syx(const char *arg);

#endif
