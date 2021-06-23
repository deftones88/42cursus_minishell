#include "minishell.h"

int  check_builtin(char *arg)
{
  return (!ft_strncmp(arg, "cd", (int)ft_strlen(arg))
  || !ft_strncmp(arg, "echo", (int)ft_strlen(arg))
  || !ft_strncmp(arg, "pwd", (int)ft_strlen(arg))
  || !ft_strncmp(arg, "export", (int)ft_strlen(arg))
  || !ft_strncmp(arg, "unset", (int)ft_strlen(arg))
  || !ft_strncmp(arg, "env", (int)ft_strlen(arg))
  || !ft_strncmp(arg, "exit", (int)ft_strlen(arg)));
}
