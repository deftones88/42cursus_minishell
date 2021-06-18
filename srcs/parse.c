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
