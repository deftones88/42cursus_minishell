# include "minishell.h"

extern int		g_ret;

void	heredoc_child(int fd[2], char *delimiter)
{
	char	*line;

	close(fd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line)
			exit(1);
		if (!ft_strcmp(line, delimiter))
		{
			free(line);
			close(fd[1]);
			exit(EXIT_SUCCESS);
		}
		write(fd[1], line, (int)ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
}

int		heredoc_eof(char *buf, char *delimiter)
{
	struct termios	t_before;
	int		k;

	k = -1;
	while (*buf == '<' || *buf == ' ')
		*buf++ = ' ';
	while (++k < (int)ft_strlen(delimiter))
		buf[k] = ' ';
	tcgetattr(STDIN_FILENO, &t_before);
	set_termcap(2);
	tcsetattr(STDIN_FILENO, TCSANOW, &t_before);
	return (1);
}

int		heredoc_parent(t_cmd *cmd, char *buf, int fd[2], int flag)
{
	char	buffer[100 + 1];
	int		status;
	int		ret;

	close(fd[1]);
	wait(&status);
	ret = WEXITSTATUS(status);
	if (flag)
		cmd->delimit = fd[0];
	else
		cmd->ret = 1;
	if (ret == 1)
		return (heredoc_eof(buf, cmd->parse[0]));
	else if (cmd->delimit < 0)
	{
		while (read(fd[0], buffer, 100))
			printf("%s", buffer);
		close(fd[0]);
	}
	return (0);
}

int		heredoc_all(t_cmd *cmd, char *buf, int i)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		err_msg("pipe failed\n");
	pid = fork();
	if (pid < 0)
		err_msg("fork failed\n");
	if (pid == 0)
		heredoc_child(fd, cmd->parse[0]);
	return (heredoc_parent(cmd, buf + i, fd, i));
}

int		redin(t_cmd *cmd)
{
	if (cmd->redin > -1)
		close(cmd->redin);
	cmd->redin = open(cmd->parse[0], O_RDONLY, 0);
	if (cmd->redin < 0)
	{
		printf("minishell: %s: %s\n", cmd->parse[0], strerror(errno));
		cmd->ret = 1;
		g_ret = errno;
		free_all(cmd->parse);
		exit(EXIT_SUCCESS);
		return (1);
	}
	return (0);
}

int		redout_append(t_cmd *cmd, int *this, int *other, int flag)
{
	if (*this > -1)
		close(*this);
	*this = open(cmd->parse[0], O_CREAT | flag | O_WRONLY, 0644);
	if (*this < 0)
	{
		g_ret = *this;
		cmd->ret = 1;
		exit(EXIT_SUCCESS);
		return (1);
	}
	if (*other > -1)
	{
		close(*other);
		*other = -1;
	}
	return (0);
}
