#ifndef UTILS_H
# define UTILS_H

# include <unistd.h>
# include <stdlib.h>

int		ft_strcmp(const char *s1, const char *s2);
int		ft_isspdigit(char *str);
int		ft_atoi(char *str, int *i);
void	err_msg(char *str);

#endif
