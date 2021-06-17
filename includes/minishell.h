#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef STDIN
#  define STDIN 1
# endif

# include "get_next_line.h"
# include "libft.h"

# include <stdio.h>
# include <list.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct	s_cmd
{
	char	*cmd;
	int		flag;
	char	*redin;
	char	*redout;

}				t_cmd;

#endif
