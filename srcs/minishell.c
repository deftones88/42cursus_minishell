/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:16:37 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/05 19:14:33 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_ret = 0;

void	show_logo(void)
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

			char	**tmp;
			int		total;
			pid_t	*pid;
			int		pipe_fdin[2];
			int		pipe_fdout[2];
			int		fd_backup[2];

			tmp = ft_split(line, "|");
			total = 0;
			while (tmp[total])
				total++;
			if (total > 1 && PRINT)
				printf("total: %d\n", total);
			pid = malloc(sizeof(pid_t) * total);
			if (total > 1)
			{
				pipe(pipe_fdin);
				pipe(pipe_fdout);
				fd_backup[0] = dup(STDIN_FILENO);
				fd_backup[1] = dup(STDOUT_FILENO);
			}
			for (int i = 0; i < total; i++)
			{
				if (total > 1 && i < total - 1 && PRINT)
				{
					printf("\t\t\t\e[34m--  in[READ]: %d,  in[WRITE]: %d\e[0m\n", pipe_fdin[0], pipe_fdin[1]);
					printf("\t\t\t\e[34m-- out[READ]: %d, out[WRITE]: %d\e[0m\n", pipe_fdout[0], pipe_fdout[1]);
				}
				pid[i] = fork();
				if (total > 1 && PRINT)
					printf("\e[31m====\t< FORK(%d) > : %d (%4d)\t====\n\e[0m", i, getpid(), getppid());
				if (pid[i] == 0)
				{
					if (total > 1)
					{
						if (PRINT)
							printf("\e[1;34m-- CLOSE fd[%d] - child\e[0;0m\n", pipe_fdin[1]);
						close(pipe_fdin[1]);
						if (i > 0)
						{
							if (PRINT)
								printf("\e[34m-- IN    fd[%d] - child\e[0m\n", pipe_fdin[0]);
							dup2(pipe_fdin[0], STDIN_FILENO);
							close(pipe_fdin[0]);
						}
						else
						{
							if (PRINT)
								printf("\e[1;34m-- CLOSE fd[%d] - child\e[0;0m\n", pipe_fdout[0]);
							close(pipe_fdout[0]);
						}
						if (i < total - 1)
						{
							if (PRINT)
								printf("\e[34m-- OUT   fd[%d] - child\e[0m\n", pipe_fdout[1]);
							dup2(pipe_fdout[1], STDOUT_FILENO);
							close(pipe_fdout[1]);
						}
						else
						{
							if (PRINT)
								printf("\e[34m-- OUT   backup\e[0m\n");
							dup2(fd_backup[1], STDOUT_FILENO);
							close(fd_backup[1]);
						}
					}
					cmd = init_cmd(tmp[i], envl);
					if (!cmd || cmd->ret > 0)
						continue ;
					while (cmd)
					{
						if (total > 1 && PRINT)
							printf("\t -. %-6s /child/ :\t%d (%4d)\n", cmd->arg[0], getpid(), getppid());
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
							ft_exec(cmd, envl, total);
						cmd = free_next(cmd);
					}
					if (total > 1 && PRINT)
						printf("\t< cmd >\t\t>> child exit - (%d)\n", i);
					exit(EXIT_SUCCESS);
				}
				else
				{
					int		status;
					waitpid(pid[i], &status, 0);
					if (total > 1 && PRINT)
						printf("\t -.        /parent/ :\t%d (%4d)\n", getpid(), getppid());
					if (total > 1)
					{
						if (i < total - 1)
						{
							if (PRINT)
								printf("\e[1;34m-- CLOSE fd[%d] - parent\e[0;0m\n", pipe_fdout[1]);
							close(pipe_fdout[1]);
							char	buf;
							while (read(pipe_fdout[0], &buf, 1) > 0)
								write(pipe_fdin[1], &buf, 1);
							if (PRINT)
								printf("\e[1;34m-- CLOSE fd[%d] - parent\e[0;0m\n", pipe_fdin[1]);
							close(pipe_fdin[1]);
						}
						else
						{
							if (PRINT)
								printf("\e[1;34m-- CLOSE fd[%d] - parent\e[0;0m\n", pipe_fdin[0]);
							close(pipe_fdin[0]);
							if (PRINT)
								printf("\e[1;34m-- CLOSE fd[%d] - parent\e[0;0m\n", pipe_fdout[0]);
							close(pipe_fdout[0]);
						}
					}
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
			if (total > 1)
			{
				dup2(fd_backup[0], STDIN_FILENO);
				if (PRINT)
					printf("\e[34m-- IN backup\e[0m\n");
				close(fd_backup[0]);
			}
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
