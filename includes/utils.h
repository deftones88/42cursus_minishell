/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:12:19 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/05 15:12:20 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <unistd.h>
# include <stdlib.h>

void	free_all(char **strs);
int	ft_strcmp(const char *s1, const char *s2);
int	ft_isspdigit(char *str);
void	err_msg(char *str);
int	check_builtin(char *arg, char *builtin);

#endif
