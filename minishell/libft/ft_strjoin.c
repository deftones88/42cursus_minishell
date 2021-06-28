/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 22:07:40 by jinukim           #+#    #+#             */
/*   Updated: 2020/09/30 22:33:03 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*p;
	int		i;
	int		idx;

	idx = 0;
	i = -1;
	p = (char*)ft_calloc((s1 ? ft_strlen(s1) : 0) + (s2 ? ft_strlen(s2) : 0)
			+ 1, sizeof(char));
	if (!p)
		return (p);
	if (s1)
		while (s1[++i])
			p[idx++] = s1[i];
	i = -1;
	if (s2)
		while (s2[++i])
			p[idx++] = s2[i];
	p[idx] = 0;
	return (p);
}
