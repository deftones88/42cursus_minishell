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
	int		str_len[2];

	idx = 0;
	i = -1;
	str_len[0] = 0;
	str_len[1] = 0;
	if (s1)
		str_len[0] = ft_strlen(s1);
	if (s2)
		str_len[1] = ft_strlen(s2);
	p = (char *)ft_calloc(str_len[0] + str_len[1] + 1, sizeof(char));
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
