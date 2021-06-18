#include "minishell.h"

//
void cmd_print(t_cmd *cmd)
{
  printf("cmd:\t%s\n", cmd->cmd);
  printf("arg:");
  for(int i = 0; cmd->arg[i]; i++)
    printf("\t- %s\n", cmd->arg[i]);
  printf("redin:\t%s\n", cmd->redin);
  printf("redout:\t%s\n", cmd->redout);
  printf("append:\t%s\n", cmd->append);
  printf("--\n");
}
//

void init_cmd(t_cmd *cmd)
{
  cmd->cmd = 0;
  cmd->arg = 0;
  cmd->flag = 0;
  cmd->redin = 0;
  cmd->redout = 0;
  cmd->append = 0;
}

void parse_tmp(char *line, t_cmd *cmd)
{
  char  **str_split;
  int   i;
  int   idx;
  int   j;

  str_split = ft_split(line, ' ');
  idx = -2;
  i = -1;
  while (str_split[++i])
    if (str_split[i][0] == '<' || str_split[i][0] == '>')
      idx = i;
  if (idx > -1)
    i -= 1;
  else
    i++;
  cmd->arg = ft_calloc(sizeof(char *), i);
  if (!cmd->arg)
    return ;
  i = -1;
  j = -1;
  while (str_split[++i])
  {
    if (i == idx || i == idx + 1)
      continue ;
    cmd->arg[++j] = ft_strdup(str_split[i]);
  }
  // set cmd->cmd
  char  **tmp_dir;
  tmp_dir = ft_split("/bin/,/usr/local/bin/,/usr/bin/,/usr/sbin/,/sbin/", ',');
  char  *tmp_join;
  int   fd;
  i = -1;
  while (++i < 5)
  {
    tmp_join = ft_strjoin(tmp_dir[i], cmd->arg[0]);
    if ((fd = open(tmp_join, O_RDONLY, 0644)) > -1)
    {
      cmd->cmd = tmp_join;
      close(fd);
      break ;
    }
    else
      free(tmp_join);
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
  freeall(0, str_split);
  freeall(0, tmp_dir);

  printf("\n--\t<<PARSE>>\n");
  cmd_print(cmd);
}
