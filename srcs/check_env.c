#include "minishell.h"

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
    // printf("1: %c(%d), flag: %d\n", line[i], i, flag);
    close_flag = 0;
    j = 0;
    if ((line[i] == ' ' && !flag) || (i > 0 && line[i] == '\"'))
      return (0);
    // printf("2\n");
    if (line[i] == '$')
    {
      // printf("2 - 1\n");
      i++;
      if (line[i] == '?')
      {
        cmd->env.env_ret++;
        cmd->env.len = ft_numlen(cmd->ret) - 2;
        cmd->env.env_str = ft_calloc(sizeof(char), ft_numlen(cmd->ret) + 1);
        if (!cmd->env.env_str)
        {
          printf("Malloc failed : %s\n", strerror(errno));
          exit(errno);
        }
        cmd->env.env_str = ft_itoa(cmd->ret);
        return (2);
      }
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

char check_env_syx(const char *arg)
{
  int   i;

  i = -1;
  while (arg[++i])
  {
    if (arg[0] == ' ' || arg[0] == '=')
      return (1);
    if (arg[i] == '=')
      break ;
  }
  if (!arg[i])
    return (2);
  return (0);
}
