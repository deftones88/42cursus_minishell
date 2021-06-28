#include "libft.h"

int		ft_isspace(int c)
{
	if (c > 0xff)
		return (0);
	return (c == ' ' || c == '\t' || c == '\n'
			|| c == '\f' || c == '\r' || c == '\v');
}
