#include "libft.h"

char	**ft_strap(char **strs)
{
	int		i;
	int		j;
	int		k;
	char	quot;
	char	buf[16384];

	i = -1;
	quot = 0;
	while (strs[++i])
	{
		j = -1;
		k = 0;
		while (strs[i][++j])
		{
			if (!quot && (strs[i][j] == '\'' || strs[i][j] == '\"'))
				quot = strs[i][j];
			else if (quot && strs[i][j] == quot)
				quot = 0;
			else
				buf[k++] = strs[i][j];
		}
		buf[k] = 0;
		free(strs[i]);
		strs[i] = ft_strdup(buf);
	}
	return (strs);
}
