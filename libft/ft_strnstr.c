/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 20:19:47 by jinukim           #+#    #+#             */
/*   Updated: 2020/09/30 20:42:38 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t l)
{
	size_t	idx;
	size_t	n;

	n = 0;
	idx = 0;
	if (to_find[0] == 0)
		return ((char *)str);
	while (str[idx] && idx < l)
	{
		while (to_find[n])
		{
			if (str[idx + n] != to_find[n] || idx + n >= l)
			{
				n = 0;
				break ;
			}
			n++;
		}
		if (n)
			return ((char *)str + idx);
		idx++;
	}
	return (0);
}
