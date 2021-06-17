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

static int		countstrs(const char *str, char c)
{
	int		i;
	int		flag;
	int		n;

	n = 0;
	i = -1;
	flag = 0;
	if (!str)
		return (0);
	while (str[++i])
		if (str[i] == c)
		{
			if (flag == 1)
			{
				n++;
				flag = 0;
			}
		}
		else
			flag = 1;
	if (flag == 1)
		n++;
	return (n);
}

static char		*istr(int *k, const char *str, char c)
{
	int		n;
	char	*ret;
	int		i;

	n = 0;
	while (str[*k + n] && str[*k + n] != c)
		n++;
	if (!(ret = (char*)malloc((n + 1) * sizeof(char))))
		return (0);
	ret[n] = 0;
	*k += n;
	i = 0;
	while (--n >= 0)
		ret[n] = str[*k - ++i];
	return (ret);
}

char			**ft_split(char const *str, char c)
{
	int		k;
	int		i;
	int		n;
	char	**ret;

	n = countstrs(str, c);
	if (!(ret = (char**)ft_calloc((n + 1), sizeof(char*))))
		return (0);
	i = -1;
	k = 0;
	while (++i < n)
	{
		while (str[k] && str[k] == c)
			k++;
		if (!(ret[i] = istr(&k, str, c)))
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
