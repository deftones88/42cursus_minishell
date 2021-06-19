#include "minishell.h"

//
void cmd_print(t_cmd *cmd)
{
  printf("cmd:\t|%s|\n", cmd->cmd);
  printf("arg:");
  for(int i = 0; cmd->arg[i]; i++)
    printf("\t- |%s|\n", cmd->arg[i]);
  printf("redin:\t|%s|\n", cmd->redin);
  printf("redout:\t|%s|\n", cmd->redout);
  printf("append:\t|%s|\n", cmd->append);
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

int  closing_quotation_check(char *line, char c, int *a)
{
  int   i;

  i = -1;
  while (line[++i])
    if (line[i] == c)
      break ;
  if (line[i])
    a++;
  if (line[i] && i > 1)
    return (i);
  return (0);
}

char **split_line(char *line)
{
  char **tmp;
  int  i;
  int  dbl[2];
  int  sgl[2];
  int  count;
  int  flag;

  i = -1;
  dbl[0] = 0;
  sgl[0] = 0;
  count = 0;
  flag = 0;
  while (line[++i])
  {
    if (line[i] == '\"')
    {
      dbl[0]++;
      i += closing_quotation_check(line + i + 1, '\"', &dbl[0]);
      count++;
    }
    else if (line[i] == '\'')
    {
      sgl[0]++;
      i += closing_quotation_check(line + i + 1, '\'', &sgl[0]);
      count++;
    }
    else if (line[i] == ' ')
    {
      if (flag)
      {
        flag = 0;
        count++;
      }
    }
    else
      flag = 1;
  }
  if (flag)
    count++;
  if (dbl[0] < 2 && sgl[0] < 2)
    return (ft_split(line, ' '));

  // printf("count:  %d\n", count);
  tmp = ft_calloc(sizeof(char *), count + 1);
  i = 0;
  int   wcount = 0;
  int   q_flag = 0;
  while (line[i])
  {
    i += q_flag;
    q_flag = 0;
    // printf("first) line[%d]: %c\n", i, line[i]);
    int   wlen = 0;
    while (line[i] == ' ')
      i++;
    // printf("after) line[%d]: %c\n\n", i, line[i]);
    while (line[i] != ' ' && line[i])
    {
      // printf("sgl: %d, dbl: %d\n", sgl[0], dbl[0]);
      if ((line[i] == '\'' && sgl[0] > 1) || (line[i] == '\"' && dbl[0] > 1))
      {
        if (line[i] == '\'')
        {
          sgl[1] = closing_quotation_check(line + i + 1, '\'', 0);
          wlen = sgl[1];
          sgl[0] -= 2;
        }
        else
        {
          dbl[1] = closing_quotation_check(line + i + 1, '\"', 0);
          wlen = dbl[1];
          dbl[0] -= 2;
        }
        if (wlen)
        {
          i += wlen + 1;
          q_flag = 1;
        }
        else
          i += 2;
        break ;
      }
      wlen++;
      i++;
    }
    if (wlen)
    {
      tmp[wcount] = ft_calloc(sizeof(char), wlen + 1);
      int  j = -1;
      // printf("tmp[%d]: |", wcount);
      while (++j < wlen)
      {
        tmp[wcount][j] = line[i - wlen + j];
        // printf("%c", tmp[wcount][j]);
      }
      // printf("| (%d)\n\n", wlen);
      wcount++;
    }
  }
  // printf("\n--\ttmp string\n");
  // for (int i = 0; tmp[i] ; i++)
  //   printf("||%s||\n", tmp[i]);
  return (tmp);
}

void parse_tmp(char *line, t_cmd *cmd)
{
  char  **str_split;
  int   i;
  int   idx;
  int   j;

  str_split = split_line(line);
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
