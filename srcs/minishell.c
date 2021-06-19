#include "minishell.h"

/*
**	<< BUILTIN COMMANDS THAT ARE EXECUTABLE WITH execve() >>
**
**  - pwd
** 	- echo '-n'
**	- env
*/

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
		dir = ft_strrchr(getcwd(NULL, 0), '/') + 1;
		dir = ft_strjoin(dir, "$ ");
		// printf("%s $ ", dir);
		line = readline(dir);
		if (ft_strlen(line) > 0)
			add_history(line);
		// if (get_next_line(STDOUT_FILENO, &line) != 1 || !line)
		// {
		// 	if (line)
		// 		free(line);
		// 	break ;
		// }
		init_cmd(&cmd);
		parse_tmp(line, &cmd);

		ft_lstadd_front(&history, ft_lstnew(line));
		pid_t pid = fork();
		if (pid == 0)
		{
			if (execve(cmd.cmd, cmd.arg, envp) == -1)
				printf("Error executing : %s\n", strerror(errno));
			exit(0);
		}
		wait(&status);
		free(line);
	}
	ft_lstclear(&history);
}
