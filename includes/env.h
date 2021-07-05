/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:09:57 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/05 15:10:01 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "list.h"

char		**list2arr(t_list *envl);
char		*parse_key(char *str);
char		*parse_value(char *str);
char		*find_value(t_list *envl, char *key);
t_list		*init_env(char **envp);

#endif
