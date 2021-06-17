/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 23:08:17 by jinukim           #+#    #+#             */
/*   Updated: 2020/09/30 23:45:26 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isin(char c, char const *set)
{
	int		i;

	i = -1;
	while (set[++i])
		if (c == set[i])
			return (1);
	return (0);
}

char		*ft_strtrim(char const *s1, char const *set)
{
	char	*p;
	int		i;
	int		j;
	int		idx;

	if (!s1)
		return (0);
	i = -1;
	while (s1[++i])
		if (!isin(s1[i], set))
			break ;
	if (s1[i] == 0)
		return ((char*)ft_calloc(1, sizeof(char)));
	j = ft_strlen(s1);
	while (s1[--j])
		if (!isin(s1[j], set))
			break ;
	p = (char*)ft_calloc(j - i + 2, sizeof(char));
	if (!p)
		return (p);
	idx = 0;
	while (i <= j)
		p[idx++] = s1[i++];
	return (p);
}
