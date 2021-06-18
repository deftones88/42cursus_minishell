#include "minishell.h"

int		main(int argc, char **argv, char **envp)
{
	char	*line;
	char	*dir;
	t_list	*history;
	t_cmd	cmd;
	int		status;

	history = 0;
	setbuf(stdout, NULL);

	//for debug
	// char **tmp;

	while(1)
	{
		dir = "current dir";
		printf("%s$ ", dir);
		if (get_next_line(STDOUT_FILENO, &line) != 1 || !line)
		{
			if (line)
				free(line);
			break ;
		}
		parse_tmp(line, &cmd);

		printf("\n--\t<<MAIN>>\n");
		cmd_print(&cmd);

		ft_lstadd_front(&history, ft_lstnew(line));
		pid_t pid = fork();
		if (pid == 0)
		{
			if (execve(cmd.cmd, cmd.arg, envp) == -1)
				perror(cmd.cmd);
			exit(0);
		}
		wait(&status);
		free(line);
	}
	ft_lstclear(&history);
}
