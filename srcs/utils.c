/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:18:02 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/05 15:18:04 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	free_all(char **strs)
{
	int		i;

	i = -1;
	while (strs[++i])
		free(strs[i]);
	free(strs);
}

int		ft_strcmp(const char *s1, const char *s2)
{
	int				ret;
	int				idx;

	ret = 0;
	idx = 0;
	while (!ret && (s1[idx] || s2[idx]))
	{
		ret = (unsigned char)s1[idx] - (unsigned char)s2[idx];
		idx++;
	}
	return (ret);
}

int		ft_isspdigit(char *str)
{
	int				i;

	i = -1;
	while (str[++i])
	{
		if ((str[i] == '-' || str[i] == '+')
				&& (str[i + 1] >= '0' && str[i + 1] <= '9')
				&& (i == 0 || (str[i - 1] < '0' && str[i - 1] > '9')))
			i++;
		else if (str[i] != ' ' && str[i] != '\t' && (str[i] < '0' || str[i] > '9'))
			return (0);
	}
	return (1);
}

void	err_msg(char *str)
{
	int				i;

	i = -1;
	while (str[++i])
		;
	write(2, str, i);
	exit(1);
}
