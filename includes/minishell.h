#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct		s_cmd
{
	char			*cmd;
	char			**arg;				// command + options
	int				flag;				// represent |(1) or ;(2)
	char			*redin;				// <
	char			*redout;			// >
	char			*append;			// >>
	char			*delimit;			// <<
	int				ret;					// return value of previous execution
											// global?
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
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
// # include <dirent.h>

# ifndef ARG_MAX
#  define ARG_MAX 2048
# endif


/*
** init.c
*/
t_cmd	*init_cmd(char *line, t_list *envl);

/*
** parse.c
*/
//
void	cmd_print(t_cmd *cmd);
//
t_cmd	*free_next(t_cmd *cmd);
char	**split_line(char *line, t_cmd *cmd, t_list *envl);
void	parse_tmp(char *line, t_cmd *cmd, t_list *envl);

/*
** check_parse.c
*/
int		check_builtin(char *arg);
int		check_closing_quotation(char *line, char c, int *a);

/*
** check_env.c
*/
int		check_env(char *line, t_cmd *cmd, int flag, t_list *envl);
char	check_env_syx(const char *arg);

#endif
