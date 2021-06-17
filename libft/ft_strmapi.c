/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 23:55:27 by jinukim           #+#    #+#             */
/*   Updated: 2020/10/01 00:02:19 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*p;

	if (!s)
		return (0);
	i = -1;
	p = (char*)ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (!p)
		return (0);
	while (s[++i])
		p[i] = f(i, s[i]);
	return (p);
}
