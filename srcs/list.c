/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:13:59 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/05 15:14:01 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

t_list	*ft_lstnew(char *key, char *value)
{
	t_list	*newlst;

	if (!(newlst = (t_list*)malloc(sizeof(t_list))))
		err_msg("malloc error\n");
	newlst->key = ft_strdup(key);
	newlst->value = ft_strdup(value);
	newlst->next = 0;
	return (newlst);
}

void	ft_lstadd_last(t_list **lst, t_list *new)
{
	t_list	*tmp;

	if (!*lst)
		*lst = new;
	else
	{
		tmp = *lst;
		while (1)
		{
			if (!ft_strcmp(tmp->key, new->key))
			{
				free(tmp->value);
				tmp->value = new->value;
				free(new->key);
				free(new);
				return ;
			}
			if (!tmp->next)
				break ;
			tmp = tmp->next;
		}
		tmp->next = new;
		new->next = 0;
	}
}

void	ft_lstdel_one(t_list *lst)
{
	if (lst)
	{
		if (lst->key)
			free(lst->key);
		if (lst->value)
			free(lst->value);
		free(lst);
	}
}

void	ft_lstdel_key(t_list **lst, char *key)
{
	t_list	*cur;

	if (!*lst)
		return ;
	cur = *lst;
	if (!ft_strcmp(cur->key, key))
	{
		*lst = cur->next;
		ft_lstdel_one(cur);
		return ;
	}
	else
		ft_lstdel_key(&cur->next, key);
}

void	ft_lstclear(t_list **lst)
{
	if (!lst || !*lst)
		return ;
	if ((*lst)->next)
		ft_lstclear(&((*lst)->next));
	ft_lstdel_one(*lst);
}
