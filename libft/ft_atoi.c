/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 20:49:30 by jinukim           #+#    #+#             */
/*   Updated: 2020/10/02 18:01:07 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	trim(char *str, unsigned int i)
{
	while (1)
	{
		if (str[i] == ' ')
			;
		else if (str[i] == '\r')
			;
		else if (str[i] == '\n')
			;
		else if (str[i] == '\t')
			;
		else if (str[i] == '\v')
			;
		else if (str[i] == '\f')
			;
		else
			break ;
		i++;
	}
	return (i);
}

int	ft_atoi(char *str)
{
	int					i;
	unsigned long long	n;
	int					s;

	s = 1;
	n = 0;
	i = trim(str, 0);
	if (str[i] == '-')
	{
		s *= -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	i--;
	while (str[++i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			n = n * 10 + str[i] - '0';
		else
			break ;
	}
	return ((int)(s * n));
}
