/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 21:53:51 by jinukim           #+#    #+#             */
/*   Updated: 2020/10/04 20:07:10 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*p;
	size_t	i;
	size_t	l;

	if (!s)
		return (0);
	l = ft_strlen(s);
	i = 0;
	p = (char*)ft_calloc(l - start > len ? len + 1 : l - start + 1,
			sizeof(char));
	if (!p || start >= ft_strlen(s))
		return (p);
	while (s[start] && i < len)
		p[i++] = s[start++];
	p[i] = 0;
	return (p);
}
