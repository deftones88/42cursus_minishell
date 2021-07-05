/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 20:04:00 by jinukim           #+#    #+#             */
/*   Updated: 2020/10/05 02:06:17 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	d;
	size_t	i;
	size_t	n;
	size_t	t;

	n = size;
	i = -1;
	d = 0;
	while (n-- != 0 && dst[d])
		d++;
	n = size - d;
	if (n == 0)
		return (d + ft_strlen(src));
	t = d;
	while (src[++i])
	{
		if (n != 1)
		{
			dst[d + i] = src[i];
			t = d + i + 1;
			n--;
		}
	}
	dst[t] = 0;
	return (d + i);
}
