#ifndef LIST_H
# define LIST_H

# include <stdlib.h>

typedef struct		s_list
{
	char			*cmd;
	struct s_list	*next;
	struct s_list	*bef;
}					t_list;

t_list				*ft_lstnew(char *cmd);
void				ft_lstadd_front(t_list **lst, t_list *new);
void				ft_lstadd_last(t_list **lst, t_list *new);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstdel_front(t_list **lst);
void				ft_lstclear(t_list **lst);

#endif
