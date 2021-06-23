#include "minishell.h"

char	check_env_syx(const char *arg)
{
	int		i;

	i = -1;
	while (arg[++i])
	{
		if (arg[0] == ' ' || arg[0] == '=')
			return (1);
		if (arg[i] == '=')
			break ;
	}
	if (!arg[i])
		return (2);
	return (0);
}
