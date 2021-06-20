#include "minishell.h"

int  check_builtin(char *arg)
{
  return (!ft_strncmp(arg, "cd", 2)
  //|| !ft_strncmp(arg, "echo", 4)
  || !ft_strncmp(arg, "pwd", 3)
  || !ft_strncmp(arg, "export", 6)
  || !ft_strncmp(arg, "unset", 5)
  || !ft_strncmp(arg, "env", 3)
  || !ft_strncmp(arg, "exit", 4));
}

int  check_closing_quotation(char *line, char c, int *a)
{
  int   i;

  i = -1;
  while (line[++i])
    if (line[i] == c)
      break ;
  if (line[i])
    ++a;
  if (line[i] && i > 1)
    return (i);
  return (0);
}

int  check_env(char *line, t_cmd *cmd, int flag)
{
  int   i;
  int   j;
  int   close_flag;
  char  buf[ARG_MAX];

  cmd->env.len = 0;
  i = -1;
  while (line[++i])
  {
    // printf("1: %d\n", i);
    close_flag = 0;
    j = 0;
    if (line[i] == ' ' && !flag)
      return (0);
    // printf("2\n");
    if (line[i] == '$')
    {
      // printf("2 - 1\n");
      i++;
      if (line[i] == '{' && ft_strchr(line + i + 1, '}'))
      {
        // printf("2 - 2\n");
        close_flag = 2;
        i++;
      }
      while (((!close_flag && (line[i + j] != ' ' && line[i + j] != '$' && line[i + j] != '\"' && line[i + j] != '\'')) || (close_flag && line[i + j] != '}')) && line[i + j])
      {
        // printf("2 - 3: %c\n", line[i + j]);
        buf[j] = line[i + j];
        j++;
      }
      buf[j] = 0;
      // printf("2 - 4: %s(%lu)\n", buf, strlen(buf));
      cmd->env.env_str = getenv(buf);
      // printf("2 - 5: %s\n", cmd->env.env_str);
      if (cmd->env.env_str)
        cmd->env.len += (int)ft_strlen(cmd->env.env_str);
      // printf("2 - 6: %d\n", cmd->env.len);
      cmd->env.len -= 1 + close_flag + (int)ft_strlen(buf);
      // printf("2 - 7: %d\n", cmd->env.len);
      return ((int)ft_strlen(buf) + close_flag + 1);
    }
    // printf("3\n");
    i += j;
  }
  return (0);
}
