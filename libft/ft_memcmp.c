/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 19:31:40 by jinukim           #+#    #+#             */
/*   Updated: 2020/09/30 19:39:06 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	int		r;
	size_t	i;

	r = 0;
	i = 0;
	while (i++ < n && !r)
		r = *(unsigned char *)s1++ - *(unsigned char *)s2++;
	return (r);
}
