#include "minishell.h"

void cmd_print(t_cmd *cmd)
{
  printf("cmd:\t%s\n", cmd->cmd);
  for(int i = 0; cmd->arg[i]; i++)
    printf("- %s\n", cmd->arg[i]);
  printf("redin:\t%s\n", cmd->redin);
  printf("redout:\t%s\n", cmd->redout);
  printf("append:\t%s\n", cmd->append);
}


int  check_arg(char c)
{
  return (c == '<' || c == '>');
}

void parse_tmp(char *line, t_cmd *cmd)
{
  char  **str_split;
  char  **tmp_dir;
  char  *tmp_join;
  int   fd;
  int   i;
  int   idx;
  int   j;

  str_split = ft_split(line, ' ');
  idx = -2;
  i = -1;
  while (str_split[++i])
    if (str_split[i][0] == '<' || str_split[i][0] == '>')
      idx = i;
  cmd = ft_calloc(sizeof(t_cmd), 1);
  // set cmd->arg
  cmd->arg = ft_calloc(sizeof(char *), i - 1);
  i = -1;
  j = -1;
  while (str_split[++i])
  {
    if (i == idx || i == idx + 1)
      continue ;
    cmd->arg[++j] = ft_strdup(str_split[i]);
  }
  // set cmd->cmd
  tmp_dir = ft_split("/bin/,/usr/local/bin/,/usr/bin/,/usr/sbin/,/sbin/", ',');
  i = -1;
  while (++i < 5)
  {
    tmp_join = ft_strjoin(tmp_dir[i], cmd->arg[0]);
    if ((fd = open(tmp_join, O_RDONLY, 0644)) > -1)
      cmd->cmd = tmp_join;
    else
      free(tmp_join);
    close(fd);
  }
  // set redirection
  if (idx > -1)
  {
    if (str_split[idx][0] == '<')
      cmd->redin = ft_strdup(str_split[idx + 1]);
    else if (str_split[idx][0] == '>')
    {
      if (str_split[idx][1] == '>')
        cmd->append = ft_strdup(str_split[idx + 1]);
      else
        cmd->redout = ft_strdup(str_split[idx + 1]);
    }
  }
  freeall(0, tmp_dir);
  freeall(0, str_split);

  printf("parse\n");
  cmd_print(cmd);
}
/*
void parse_tmp(char *line, t_cmd *cmd)
{
  int   idx;
  int   i;
  char  *tmp_str;
  char  *tmp_join;
  char  **tmp_dir;
  int   fd;

  cmd = ft_calloc(sizeof(t_cmd), 1);
  idx = -1;
  while (line[++idx])
    if (check_arg(line[idx]))
      break ;
  // set cmd->arg
  tmp_str = malloc(sizeof(char) * (idx + 1));
  ft_strlcpy(tmp_str, line, idx + 1);
  cmd->arg = ft_split(tmp_str, ' ');
  free(tmp_str);
  // set cmd->cmd
  tmp_dir = ft_split("/bin/,/usr/local/bin/,/usr/bin/,/usr/sbin/,/sbin/", ',');
  i = -1;
  while (++i < 5)
  {
    tmp_join = ft_strjoin(tmp_dir[i], cmd->arg[0]);
    if ((fd = open(tmp_join, O_RDONLY, 0644)) > -1)
      cmd->cmd = tmp_join;
    else
      free(tmp_join);
    close(fd);
  }
  freeall(0, tmp_dir);
  // set cmd->redin
  tmp_dir = ft_split(line + idx + 1, ' ');
  // printf("line[%d]: %c\n", idx, line[idx]);
  // for(int i = 0; tmp_dir[i]; i++)
    // printf("- %s\n", tmp_dir[i]);
  if (line[idx] == '<')
    cmd->redin = ft_strdup(tmp_dir[0]);
  else if (line[idx] == '>')
  {
    if (line[idx + 1] == '>')
      cmd->append = ft_strdup(tmp_dir[1]);
    else
      cmd->redout = ft_strdup(tmp_dir[0]);
  }
  freeall(0, tmp_dir);


  cmd_print(cmd);
}
*/
