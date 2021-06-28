#ifndef BUILTIN_H
# define BUILTIN_H

# include "list.h"
# include "minishell.h"

void	ft_exec(t_cmd *cmd, t_list *envl);
void	builtin_echo(t_cmd *cmd);
void	builtin_export(t_cmd *cmd, t_list **envl);
void	builtin_unset(t_cmd *cmd, t_list  **envl);
void	builtin_env(t_cmd *cmd, t_list *envl, int flag);

#endif
