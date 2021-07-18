/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:10:15 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/05 15:10:18 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H

# include <stdlib.h>
# include "libft.h"
# include "utils.h"

typedef struct s_list
{
	char			*key;
	char			*value;
	struct s_list	*next;
}					t_list;

t_list				*ft_lstnew(char *key, char *value);
void				ft_lstadd_last(t_list **lst, t_list *new);
void				ft_lstdel_one(t_list *lst);
void				ft_lstdel_key(t_list **lst, char *key);
void				ft_lstclear(t_list **lst);

#endif
