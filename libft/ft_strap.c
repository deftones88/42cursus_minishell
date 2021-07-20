#include "libft.h"

static void	init(int *a, char *b)
{
	*a = 0;
	*b = 0;
}

char	**ft_strap(char **strs)
{
	int		i;
	int		j;
	int		k;
	char	quot;
	char	buf[16384];

	init(&i, &quot);
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
