#include "minishell.h"

void show_logo(void)
{
	printf("\n");
	printf("-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-\n");
	printf("                                                   ,-.\n");
	printf("           ╔╗        ╔╗╔╗                       _.|  '\n");
	printf("          ╔╬╣ ╔═╦╦╦╗ ║╠╬╬══╗                  .'  | /\n");
	printf("          ╠╣╠═╚╩═╩═╝═╣═╣║║║║               ,'    |'\n");
	printf("         ╔╝╠╝        ╚╩╩╩╩╩╝              /      /\n");
	printf("         ╚═╝              _..----\"\"---.'      /\n");
	printf("    _.....---------...,-\"\"                  ,'\n");
	printf("    `-._  \\                                /\n");
	printf("        `-.+_            __           ,--. .\n");
	printf("             `-.._     .:  ).        (`--\"| \\\n");
	printf("                  7    | `\" |         `...'  \\\n");
	printf("                  |     `--'     '+\"        ,\". ,\"\"-\n");
	printf("                  |   _...        .____     | |/    '\n");
	printf("             _.   |  .    `.  '--\"   /      `./     j\n");
	printf("            \\' `-.|  '     |   `.   /        /     /\n");
	printf("            '     `-. `---\"      `-\"        /     /\n");
	printf("  __^__      \\       `.                  _,'     /        __^__\n");
	printf(" ( ___ )-------------------------------------------------( ___ )\n");
	printf("  | / | ==================╔╗==╔╦═╦╗=======================| \\ |\n");
	printf("  | / |                ╔══╬╬═╦╬╣═╣╚╦═╦╗╔╗                 | \\ |\n");
	printf("  | / |         *      ║║║║║║║║╠═║║║╩╣╚╣╚╗      *         | \\ |\n");
	printf("  |___| ===============╚╩╩╩╩╩═╩╩═╩╩╩═╩═╝═╝================|___|\n");
	printf(" (_____)-------------------------------------------------(_____)\n");
	printf("-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-\n\n");
}

int		g_ret = 0;

int		main(int argc, char **argv, char **envp)
{
	char	*line;
	t_cmd	*cmd;
	t_list	*envl;

	/* ctrl commands */
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);

	/* termios for muting "^C" */
	struct termios	t_old;
	tcgetattr(STDIN_FILENO, &t_old);
	set_termios(0);

	envl = init_env(envp);
	show_logo();

	while(1)
	{
		line = readline("minishell$ ");
		if (line && line[0] != 0)
		{
			add_history(line);

			char **tmp;
			int		total;
			pid_t	*pid;
			int		*fd;
			int		fd_backup[2];

			tmp = ft_split(line, "|");
			total = 0;
			while (tmp[total])
				total++;
			pid = malloc(sizeof(pid_t) * total);
			fd = malloc(sizeof(int) * ((total - 1) * 2));
			fd_backup[0] = dup(STDIN_FILENO);
			fd_backup[1] = dup(STDOUT_FILENO);
			for (int i = 0; tmp[i]; i++)
			{
				pid[i] = fork();
				printf("==\t<< FORK >> gid(%d / %d): %d[%d]\n", i, total, getpid(), getppid());
				pipe(fd + (i * 2));
				if (pid[i] == 0)
				{
					if (i > 0)
					{
						printf("\tin: fd[%d]\n", i * 2);
						dup2(fd[i * 2], STDIN_FILENO);
						close(fd[i * 2]);
					}
					if (i < total - 1)
					{
						printf("\tout: fd[%d]\n", (i * 2) + 1);
						dup2(fd[(i * 2) + 1], STDOUT_FILENO);
						close(fd[(i * 2) + 1]);
					}
					else
					{
						printf("--\tout backup\n");
						dup2(fd_backup[1], STDOUT_FILENO);
						close(fd_backup[1]);
					}
					cmd = init_cmd(tmp[i], envl);
					if (!cmd || cmd->ret > 0)
						continue ;
					while (cmd)
					{
						printf("----\tGID: %d(%d)\n", getpid(), getppid());
						if (!ft_strcmp(cmd->arg[0], "echo"))
							builtin_echo(cmd);
						else if (!ft_strcmp(cmd->arg[0], "cd"))
							builtin_cd(cmd);
						else if (!ft_strcmp(cmd->arg[0], "pwd"))
						{
							printf("%s\n", getcwd(NULL, 0));
							g_ret = 0;
						}
						else if (!ft_strcmp(cmd->arg[0], "export"))
							builtin_export(cmd, &envl);
						else if (!ft_strcmp(cmd->arg[0], "unset"))
							builtin_unset(cmd, &envl);
						else if (!ft_strcmp(cmd->arg[0], "env"))
							builtin_env(envl, 0);
						else if (!ft_strcmp(cmd->arg[0], "exit"))
						{
							tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
							exit(1);
						}
						else
							ft_exec(cmd, envl);
						cmd = free_next(cmd);
					}
					exit(EXIT_SUCCESS);
				}
				else
				{
					int		status;
					waitpid(pid[i], &status, 0);
					if (i > 0)
						close(fd[(i - 1) * 2]);
					close(fd[(i * 2) + 1]);
					if (WIFEXITED(status))
					{
						if (WEXITSTATUS(status) == 1)
						{
							wait(0);
							printf("exit\n");
							exit(EXIT_SUCCESS);
						}
						else
							g_ret = WEXITSTATUS(status);
					}
				}
			}
			printf("--\tin backup\n");
			dup2(fd_backup[0], STDIN_FILENO);
			close(fd_backup[0]);
		}
		else if (line == NULL)
		{
			set_termcap(0);
			printf("minishell$ exit\n");
			tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
			exit(EXIT_SUCCESS);
		}
		free(line);
	}
}
