#ifndef ENV_H
# define ENV_H

# include "list.h"

char	    **list2arr(t_list *envl);
char		*parse_key(char *str);
char		*parse_value(char *str);
char		*find_value(t_list *envl, char *key);
t_list		*init_env(char **envp);

#endif
