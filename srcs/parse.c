#include "minishell.h"

//
void cmd_print(t_cmd *cmd)
{
  printf("cmd:\t\t|%s|\n", cmd->cmd);
  printf("arg:");
  for(int i = 0; cmd->arg[i]; i++)
    printf("\t\t- |%s| (%lu)\n", cmd->arg[i], strlen(cmd->arg[i]));
  printf("redin:\t\t|%s|\n", cmd->redin);
  printf("redout:\t\t|%s|\n", cmd->redout);
  printf("append:\t\t|%s|\n", cmd->append);
  printf("delimit:\t|%s|\n", cmd->delimit);
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
  cmd->delimit = 0;
  // cmd->env.len = 0;
  // cmd->env.env_str = 0;
}

void free_cmd(t_cmd *cmd)
{
  freeall(cmd->cmd, cmd->arg);
  if (cmd->redin)
    free(cmd->redin);
  if (cmd->redout)
    free(cmd->redout);
  if (cmd->append)
    free(cmd->append);
  if (cmd->delimit)
    free(cmd->delimit);
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

char **split_line(char *line, t_cmd *cmd)
{
  char **tmp;
  int  i;
  int  dbl;
  int  sgl;
  int  count;
  int  flag;
  int  is_env;

  i = -1;
  dbl = 0;
  sgl = 0;
  count = 0;
  flag = 0;
  is_env = 0;
  while (line[++i])
  {
    if (line[i] == '\"')
    {
      dbl++;
      i += check_closing_quotation(line + i + 1, '\"', &dbl);
      count++;
    }
    else if (line[i] == '\'')
    {
      sgl++;
      i += check_closing_quotation(line + i + 1, '\'', &sgl);
      count++;
    }
    else if (line[i] == '$')
      is_env++;
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
  if (dbl < 2 && sgl < 2 && !is_env)
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
    // printf("i: %d\n", i);
    while (line[i] == ' ')
      i++;
    // printf("after) line[%d]: %c\n\n", i, line[i]);
    cmd->env.is_env = check_env(line + i, cmd, 0);
    // printf("env.len: %d\n", cmd->env.len);
    cmd->env.single = 0;
    while (line[i] != ' ' && line[i])
    {
      // printf("line[%d]: %c\n", i, line[i]);
      // printf("sgl: %d, dbl: %d\n", sgl, dbl);
      if ((line[i] == '\'' && sgl > 1) || (line[i] == '\"' && dbl > 1))
      {
        // printf("inside\n");
        if (line[i] == '\'')
        {
          cmd->env.len = 0;
          wlen = check_closing_quotation(line + i + 1, '\'', 0);
          sgl -= 2;
          cmd->env.single = 1;
        }
        else
        {
          cmd->env.is_env = check_env(line + i, cmd, 1);
          // printf("inside) env.len: %d\n", cmd->env.len);
          wlen += check_closing_quotation(line + i + 1, '\"', 0);
          // printf("wlen: %d\n", wlen);
          dbl -= 2;
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
      // printf("len: %d\n", wlen + 1 + cmd->env.len);
      tmp[wcount] = ft_calloc(sizeof(char), wlen + cmd->env.len + 1);
      int  j = -1;
      int  l = 0;;
      while (++j < wlen + cmd->env.len)
      {
        if (line[i - wlen + l] == '$' && !cmd->env.single)
        {
          int   k = -1;
          while (cmd->env.env_str && ++k < (int)ft_strlen(cmd->env.env_str))
          {
            tmp[wcount][j] = cmd->env.env_str[k];
            j++;
          }
          // printf("%s(%lu)\n", tmp[wcount], strlen(tmp[wcount]));
          if (j >= wlen + cmd->env.len)
          {
            // printf("j: %d\n", j);
            break ;
          }
          l += cmd->env.is_env;
        }
        // printf("tmp)line[%d]: %c\n", i - wlen + l, line[i - wlen + l]);
        tmp[wcount][j] = line[i - wlen + l];
        l++;
      }
      wcount++;
    }
  }
  return (tmp);
}

void parse_tmp(char *line, t_cmd *cmd)
{
  char  **str_split;
  int   i;
  int   idx;
  int   j;

  str_split = split_line(line, cmd);
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
    {
      if (str_split[idx][1] == '<')
        cmd->delimit = ft_strdup(str_split[idx + 1]);
      else
        cmd->redin = ft_strdup(str_split[idx + 1]);
    }
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
