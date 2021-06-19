#include "minishell.h"

/*
**	<< BUILTIN COMMANDS THAT ARE EXECUTABLE WITH execve() >>
**
**  - pwd
** 	- echo '-n'
**	- env
**
**  - 'cd' exists, but only checks if the directory exists
*/

int		main(int argc, char **argv, char **envp)
{
	char	*line;
	char	*dir;
	// t_list	*history;
	t_cmd	cmd;
	int		status;

	// history = 0;
	setbuf(stdout, NULL);

	//for debug

	while(1)
	{
		dir = ft_strrchr(getcwd(NULL, 0), '/') + 1;
		dir = ft_strjoin(dir, "$ ");
		line = readline(dir);
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			// printf("%s $ ", dir);
			// if (get_next_line(STDOUT_FILENO, &line) != 1 || !line)
			// {
				// 	if (line)
				// 		free(line);
				// 	break ;
				// }
				init_cmd(&cmd);
				parse_tmp(line, &cmd);

				// ft_lstadd_front(&history, ft_lstnew(line));
				if (cmd.cmd)
				{
					pid_t pid = fork();
					if (pid == 0)
					{
						if (execve(cmd.cmd, cmd.arg, envp) == -1)
							printf("Error executing : %s\n", strerror(errno));
						exit(0);
					}
					wait(&status);
				}
				else
					printf(">> %s\n", cmd.arg[0]);
				free_cmd(&cmd);
		}
		free(line);
		free(dir);
	}
	// ft_lstclear(&history);
}
