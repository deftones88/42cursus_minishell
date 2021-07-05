/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 22:58:14 by jinukim           #+#    #+#             */
/*   Updated: 2020/10/12 22:03:50 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isin(char n, char *str)
{
	int		i;

	i = -1;
	while (str[++i])
		if (n == str[i])
			return (1);
	return (0);
}

static int	countstrs(const char *str, char *c)
{
	int		i[2];
	int		flag;
	char	quot;

	i[0] = 0;
	i[1] = -1;
	flag = 0;
	quot = 0;
	while (str[++i[1]])
	{
		if (!quot && isin(str[i[1]], c))
		{
			if (flag == 1 && ++i[0])
				flag = 0;
		}
		else
		{
			if (!quot && (str[i[1]] == '\'' || str[i[1]] == '\"'))
				quot = str[i[1]];
			else if (quot && str[i[1]] == quot)
				quot = 0;
			flag = 1;
		}
	}
	return (i[0] + flag);
}

static char	*istr(int *k, const char *str, char *c)
{
	int		n;
	char	ret[65536];
	char	quot;

	n = 0;
	quot = 0;
	while (str[*k])
	{
		if (!quot)
		{
			if (isin(str[*k], c))
				break ;
			if (str[*k] == '\'' || str[*k] == '\"')
				quot = str[*k];
			else
				ret[n++] = str[*k];
		}
		else if (str[*k] != quot)
			ret[n++] = str[*k];
		else
			quot = 0;
		++*k;
	}
	ret[n] = 0;
	return (ft_strdup(ret));
}

char	**ft_split(char const *str, char *c)
{
	int		i[2];
	int		n;
	char	**ret;

	n = countstrs(str, c);
	ret = (char **)ft_calloc((n + 1), sizeof(char *));
	if (!ret)
		return (0);
	i[1] = -1;
	i[0] = 0;
	while (++i[1] < n)
	{
		while (str[i[0]] && isin(str[i[0]], c))
			i[0]++;
		ret[i[1]] = istr(&i[0], str, c);
		if (!ret[i[1]])
		{
			n = 0;
			while (ret[n])
				free(ret[n++]);
			free(ret);
			return (0);
		}
	}
	return (ret);
}
