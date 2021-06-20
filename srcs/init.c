#include "minishell.h"

void init_cmd(t_cmd *cmd)
{
  cmd->cmd = 0;
  cmd->arg = 0;
  cmd->flag = 0;
  cmd->redin = 0;
  cmd->redout = 0;
  cmd->append = 0;
  cmd->delimit = 0;
  // cmd->env.len = 0;
  // cmd->env.env_str = 0;
}
