#include "minishell.h"

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
	cmd.ret = 0;
	while(1)
	{
		// dir = ft_strrchr(getcwd(NULL, 0), '/') + 1;
		dir = ft_strjoin(ft_strrchr(getcwd(NULL, 0), '/') + 1, "$ ");
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
				if (!check_builtin(cmd.arg[0]))
				{
					pid_t pid = fork();
					if (pid == 0)
					{
						if (execve(cmd.arg[0], cmd.arg, envp) == -1)
						{
							printf("errno: %d (NEED to find a way to share this)\n", errno);
							// cmd.ret = errno;
							printf("Error executing : %s\n", strerror(errno));
						}
						exit(0);
					}
					wait(&status);
				}
				else
				{
					if (!ft_strcmp(cmd.arg[0], "echo"))
					{
						int		flag;
						int		i;

						flag = 0;
						i = 0;
						printf(">> echo:");
						if (!ft_strncmp(cmd.arg[1], "-n", (int)ft_strlen(cmd.arg[1])))
						{
							flag = 1;
							i++;
						}
						while (cmd.arg[++i])
							printf(" /%s/", cmd.arg[i]);
						if (!flag)
							printf("\n");
						cmd.ret = 0;
					}
					else if (!ft_strcmp(cmd.arg[0], "cd"))
					{
						printf(">> cd: %s\n", cmd.arg[1]);
						cmd.ret = chdir(cmd.arg[1]);
					}
					else if (!ft_strcmp(cmd.arg[0], "pwd"))
						printf(">> pwd: %s\n", getcwd(NULL, 0));
					else if (!ft_strcmp(cmd.arg[0], "export"))
						printf(">> export\n");
					else if (!ft_strcmp(cmd.arg[0], "unset"))
						printf(">> unset\n");
					else if (!ft_strcmp(cmd.arg[0], "env"))
					{
						/*
						** << MY SUGGESTION >>
						** create a env.txt file and copy all of **envp
						** then, keep adding to it when 'export' is called
						** erase when 'unset' is called
						** env -> read from env.txt
						*/
						printf(">> env\n");
						for (int i = 0; envp[i]; i++)
						{
							for (int j = 0; envp[i][j]; j++)
								printf("%c", envp[i][j]);
							printf("\n");
						}
						cmd.ret = 0;
					}
					else if (!ft_strcmp(cmd.arg[0], "exit"))
						exit(EXIT_SUCCESS);
				}
				free_cmd(&cmd);
		}
		free(line);
		free(dir);
	}
	// ft_lstclear(&history);
}
