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
	char **tmp;

	while(1)
	{
		dir = "current dir";
		printf("%s$ ", dir);
		if (get_next_line(STDIN, &line) != 1 || !line)
		{
			if (line)
				free(line);
			break ;
		}
//		parse_tmp(line, &cmd);

		tmp = ft_split(line, ' ');
		cmd.cmd = tmp[0];

		ft_lstadd_front(&history, ft_lstnew(line));
		pid_t pid = fork();
		if (pid == 0)
		{

			/* only execve allowed */
			execvp(cmd.cmd, tmp); //have to implement
			//execve(cmd.cmd, cmd.arg, cmd.env);
			// cmd.cmd = "wc"   			(char *)
			// cmd.arg = "wc", "-l" 	(char **)
			// cmd.env = envp?
			exit(0);
		}
		wait(&status);
		free(line);
	}
	ft_lstclear(&history);
}
