/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 17:53:03 by jinukim           #+#    #+#             */
/*   Updated: 2020/09/30 19:19:04 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *restrict dst, const void *restrict src, size_t n)
{
	int		i;

	i = 0;
	if (dst < src)
		while (i < (int)n)
		{
			*((unsigned char*)dst + i) = *((unsigned char*)src + i);
			i++;
		}
	else if (dst > src)
	{
		i = (int)n - 1;
		while (i >= 0)
		{
			*((unsigned char*)dst + i) = *((unsigned char*)src + i);
			i--;
		}
	}
	return (dst);
}
