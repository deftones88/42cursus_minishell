/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinukim <jinukim@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/14 21:28:28 by jinukim           #+#    #+#             */
/*   Updated: 2020/10/17 03:56:46 by jinukim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		get_next_line(int fd, char **line)
{
	static char	*remain[OPEN_MAX];
	char		*buf;
	int			i;

	if (fd < 0 || fd >= OPEN_MAX || !line || BUFFER_SIZE < 1)
		return (-1);
	*line = ft_dup("");
	i = ft_remainder(fd, remain, line);
	if (i != 0)
		return (i);
	buf = (char*)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buf)
		return (freeall(0, line));
	while (!remain[fd] && (i = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[i] = 0;
		if (buf[(i = ft_join(line, buf))] == '\n')
			if (!(remain[fd] = ft_dup(buf + i + 1)))
				return (freeall(buf, line));
	}
	freeall(buf, (char **)((!(i + 1)) * (unsigned long long int)line));
	return (remain[fd] ? 1 : i);
}
