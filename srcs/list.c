#include "list.h"

t_list	*ft_lstnew(char *cmd)
{
	t_list	*newlst;

	if (!(newlst = (t_list*)malloc(sizeof(t_list))))
		err_msg("malloc error\n");
	newlst->cmd = ft_strdup(cmd);
	newlst->next = 0;
	newlst->bef = 0;
	return (newlst);
}

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!*lst)
		*lst = new;
	else
	{
		new->bef = 0;
		new->next = *lst;
		(*lst)->bef = new;
		*lst = new;
	}
}

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_last(t_list **lst, t_list *new)
{
	t_list	*last;

	last = ft_lstlast(*lst);
	if (!last)
	{
		new->next = 0;
		new->bef = 0;
		*lst = new;
	}
	else
	{
		last->next = new;
		new->bef = last;
		new->next = 0;
	}
}

int		ft_lstsize(t_list *lst)
{
	int		i;
	t_list	*cur;

	i = 0;
	cur = lst;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	return (i);
}

t_list	*ft_lstdel_front(t_list **lst)
{
	t_list	*tmp;

	if (!*lst)
		return 0;
	tmp = *lst;
	tmp->next->bef = tmp->bef;
	tmp->bef->next = tmp->next;
	if (tmp == tmp->next)
		*lst = 0;
	else
		*lst = tmp->next;
	tmp->bef = tmp;
	tmp->next = tmp;
	return (tmp);
}

void	ft_lstclear(t_list **lst)
{
	if (!lst || !*lst)
		return ;
	if ((*lst)->next)
		ft_lstclear(&((*lst)->next));
	free((*lst)->cmd);
	free(*lst);
}
