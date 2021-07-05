/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 21:40:05 by jinukim           #+#    #+#             */
/*   Updated: 2020/09/30 21:42:13 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *str)
{
	int		i;
	char	*arr;

	i = -1;
	while (str[++i])
		;
	arr = (char *)malloc((i + 1) * sizeof(char));
	if (arr == 0)
		return ((char *)0);
	arr[i] = 0;
	while (--i >= 0)
		arr[i] = str[i];
	return (arr);
}
