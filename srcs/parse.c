#include "minishell.h"

//
void cmd_print(t_cmd *cmd)
{
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

void free_cmd(t_cmd *cmd)
{
	freeall(0, cmd->arg);
	if (cmd->redin)
		free(cmd->redin);
	if (cmd->redout)
		free(cmd->redout);
	if (cmd->append)
		free(cmd->append);
	if (cmd->delimit)
		free(cmd->delimit);
	if (cmd->env.env_ret)
		free(cmd->env.env_str);
	printf("--\tret: %d\n", cmd->ret);
	cmd->ret %= 256;
}

char **split_line(char *line, t_cmd *cmd, t_list *envl)
{
	int		i;
	int		b_idx;
	int		count;
	char	buf[ARG_MAX];

	i = -1;
	count = 0;
	b_idx = -1;
	ft_bzero(buf, ARG_MAX);
	while (line[++i])
	{
		if (line[0] == ' ' && i == 0)
		{
			while (line[i] == ' ')
				i++;
		}
		// printf("line[%d]: %c\n", i, line[i]);
		if (line[i] == '\'' && ft_strchr(line + i + 1, '\''))
		{
			while (line[++i] != '\'')
			{
				// printf("\' %c\n", line[i]);
				buf[++b_idx] = line[i];
			}
			i++;
		}
		else if (line[i] == '\"' && ft_strchr(line + i + 1, '\"'))
		{
			while (line[++i] != '\"')
			{
				// printf("\" %c\n", line[i]);
				if (line[i] == '$')
				{
					int		close_flag = 0;
					char	tmp[ARG_MAX];
					char	*env_str;
					int		j = 0;
					ft_bzero(tmp, ARG_MAX);
					i++;
					if (line[i] == '?')
					{
						// ret
					}
					else if (line[i] == '{' && ft_strchr(line + i + 1, '}'))
					{
						close_flag = 2;
						i++;
					}
					while (((!close_flag && (line[i + j] != ' ' && line[i + j] != '$' && line[i + j] != '\"' && line[i + j] != '\'')) || (close_flag && line[i + j] != '}')) && line[i + j])
					{
						tmp[j] = line[i + j];
						j++;
					}
					env_str = find_value(envl, tmp);
					if (env_str)
					{
						for(j = 0; env_str[j]; j++)
							buf[++b_idx] = env_str[j];
						// free(env_str);
					}
					i += (int)ft_strlen(tmp) + close_flag - (close_flag / 2);
				}
				else
					buf[++b_idx] = line[i];
			}
			i++;
		}
		else if (line[i] == '$')
		{
			int		close_flag = 0;
			char	tmp[ARG_MAX];
			char	*env_str;
			int		j = 0;
			ft_bzero(tmp, ARG_MAX);
			i++;
			if (line[i] == '?')
			{
				// ret
			}
			else if (line[i] == '{' && ft_strchr(line + i + 1, '}'))
			{
				close_flag = 2;
				i++;
			}
			while (((!close_flag && (line[i + j] != ' ' && line[i + j] != '$' && line[i + j] != '\"' && line[i + j] != '\'')) || (close_flag && line[i + j] != '}')) && line[i + j])
			{
				tmp[j] = line[i + j];
				j++;
			}
			env_str = find_value(envl, tmp);
			if (env_str)
			{
				for(j = 0; env_str[j]; j++)
					buf[++b_idx] = env_str[j];
				// free(env_str);
			}
			i += (int)ft_strlen(tmp) + close_flag - (close_flag / 2);
		}
		if (line[i] != ' ')
		{
			// printf("%c\n", line[i]);
			buf[++b_idx] = line[i];
		}
		if (line[i] == ' ' || line[i + 1] == '\0')
		{
			count++;
			buf[++b_idx] = '\0';
			while (line[i + 1] == ' ' && line[i + 1])
				i++;
		}
	}
	// printf("count: %d\n", count);
	// i = -1;
	// for (int j = 0; j < count ; j++)
	// {
	// 	while (buf[++i])
	// 		printf("%c", buf[i]);
	// 	printf("\n");
	// }
	char **tmp;

	tmp = ft_calloc(sizeof(char *), count + 1);
	int		idx = -1;
	int		mark = 0;
	while (++idx < count)
	{
		tmp[idx] = ft_strdup(buf + mark);
		mark += ft_strlen(buf + mark) + 1;
	}
	// printf("\n");
	// for (i = 0; tmp[i]; i++)
	// {
	// 	printf("tmp[%d]: %s\n", i, tmp[i]);
	// }
	return (tmp);
}
/*
char **split_line(char *line, t_cmd *cmd)
{
	int		i;
	int		dbl;
	int		sgl;
	int		count;
	int		flag;
	int		is_env;

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
	// parsing quotations
	char	**tmp;
	tmp = ft_calloc(sizeof(char *), count + 1);
	i = 0;
	int		wcount = 0;
	int		q_flag = 0;
	while (line[i])
	{
		i += q_flag;
		q_flag = 0;
		printf("first) line[%d]: %c\n", i, line[i]);
		int		wlen = 0;
		while (line[i] == ' ')
			i++;
		printf("after) line[%d]: %c\n\n", i, line[i]);
		cmd->env.is_env = check_env(line + i, cmd, 0);
		printf("env.len: %d\n", cmd->env.len);
		cmd->env.single = 0;
		while (line[i] != ' ' && line[i])
		{
			printf("line[%d]: %c\n", i, line[i]);
			printf("sgl: %d, dbl: %d\n", sgl, dbl);
			if ((line[i] == '\'' && sgl > 1) || (line[i] == '\"' && dbl > 1))
			{
				printf("inside\n");
				if (line[i] == '\'')
				{
					cmd->env.len = 0;
					wlen = check_closing_quotation(line + i + 1, '\'', 0);
					sgl -= 2;
					cmd->env.single = 1;
				}
				else
				{
					cmd->env.is_env = check_env(line + i + 1, cmd, 1);
					printf("inside) env.len: %d\n", cmd->env.len);
					wlen += check_closing_quotation(line + i + 1, '\"', 0);
					printf("wlen: %d\n", wlen);
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
			printf("len: %d\n", wlen + 1 + cmd->env.len);
			tmp[wcount] = ft_calloc(sizeof(char), wlen + cmd->env.len + 1);
			int		tmp_idx = -1;
			int		line_idx = 0;;
			while (++tmp_idx < wlen + cmd->env.len)
			{
				if (line[i - wlen + line_idx] == '$' && !cmd->env.single)
				{
					int		str_idx = -1;
					while (cmd->env.env_str && ++str_idx < (int)ft_strlen(cmd->env.env_str))
					{
						tmp[wcount][tmp_idx] = cmd->env.env_str[str_idx];
						tmp_idx++;
					}
					printf("%s(%lu)\n", tmp[wcount], strlen(tmp[wcount]));
					if (tmp_idx >= wlen + cmd->env.len)
					{
						printf("tmp_idx: %d\n", tmp_idx);
						break ;
					}
					line_idx += cmd->env.is_env;
				}
				printf("tmp)line[%d]: %c\n", i - wlen + line_idx, line[i - wlen + line_idx]);
				tmp[wcount][tmp_idx] = line[i - wlen + line_idx];
				line_idx++;
			}
			wcount++;
		}
	}
	return (tmp);

}*/

void parse_tmp(char *line, t_cmd *cmd, t_list *envl)
{
	char	**str_split;
	int		i;
	int		idx;
	int		j;

	str_split = split_line(line, cmd, envl);
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
		if (i == 0 && !check_builtin(str_split[0]))
		{
			char	**tmp_dir;
			tmp_dir = ft_split("/bin/,/usr/local/bin/,/usr/bin/,/usr/sbin/,/sbin/", ',');
			char	*tmp_join;
			int		fd;
			int		k;
			k = -1;
			while (++k < 5)
			{
				tmp_join = ft_strjoin(tmp_dir[k], str_split[i]);
				if ((fd = open(tmp_join, O_RDONLY, 0644)) > -1)
				{
					cmd->arg[++j] = tmp_join;
					close(fd);
					k = -1;
					break ;
				}
				else
				free(tmp_join);
			}
			freeall(0, tmp_dir);
			if (k > -1)
				cmd->arg[++j] = ft_strdup(str_split[i]);
			continue ;
		}
		cmd->arg[++j] = ft_strdup(str_split[i]);
	}
	// set cmd->cmd

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

	printf("\n--\t<<PARSE>>\n");
	cmd_print(cmd);
}
