#include "minishell.h"

int		main(int argc, char **argv, char **envp)
{
	char	*line;
	char	*dir;
	t_cmd	cmd;
	int		status;
	t_list	*envl;

	/* ctrl commands */
	signal(SIGINT, sig_handler); // ctrl + C
	signal(SIGQUIT, SIG_IGN);		 // ctrl + \ (only works once)


	cmd.ret = 0; 	// change to global?

	envl = init_env(envp);

	while(1)
	{
		dir = ft_strjoin(ft_strrchr(getcwd(NULL, 0), '/') + 1, "$ ");
		line = readline(dir);
		if (line && ft_strlen(line) > 0)
		{
			add_history(line);
			init_cmd(&cmd);
			parse_tmp(line, &cmd, envl);

			if (!check_builtin(cmd.arg[0]))
			{
				pid_t pid = fork();
				if (pid == 0)
				{
					if (execve(cmd.arg[0], cmd.arg, envp) == -1)
					{
						// printf("errno: %d\n", errno);
						cmd.ret = errno;
						printf("Error executing : %s\n", strerror(errno));
					}
					exit(0);
				}
				wait(&status);
			}
			else
			{
				/* builtin functions */
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
						printf(" |%s|", cmd.arg[i]);
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
				{
					printf(">> pwd: ");
					printf("%s\n", getcwd(NULL, 0));
				}
				else if (!ft_strcmp(cmd.arg[0], "export"))
				{
					printf(">> export\n");

					int		i = -1;
					while (cmd.arg[++i])
					{
						if (!check_env_syx(cmd.arg[i]))
						{
							
						}
						else if (check_env_syx(cmd.arg[i]) == 1)
							printf("%s: '%s': not a valid identifier\n", cmd.arg[0], cmd.arg[i]);
					}
				}
				else if (!ft_strcmp(cmd.arg[0], "unset"))
				{
					printf(">> unset\n");

				}
				else if (!ft_strcmp(cmd.arg[0], "env"))
				{
					printf(">> env\n");
					for (t_list *tmp = envl; tmp; tmp = tmp->next)
						printf("%s=%s\n", tmp->key, tmp->value);
					cmd.ret = 0;
				}
				else if (!ft_strcmp(cmd.arg[0], "exit"))
				{
					exit(EXIT_SUCCESS);
				}
			}
			free_cmd(&cmd);
		}
		else if (line == NULL)
		{
			/* ctrl + D (only works when readline buffer is empty) */
			printf("<< EOF >>\n");
			exit(EXIT_SUCCESS);
		}
		free(line);
		free(dir);
	}
	// ft_lstclear(&history);
}
