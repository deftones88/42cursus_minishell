/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 19:42:49 by jinukim           #+#    #+#             */
/*   Updated: 2020/10/05 00:48:07 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	idx;

	idx = 0;
	if (!dest || !src)
		return (0);
	while ((int)idx < (int)size - 1 && src[idx])
	{
		dest[idx] = src[idx];
		idx++;
	}
	if (size)
		dest[idx] = 0;
	while (src[idx])
		idx++;
	return (idx);
}
