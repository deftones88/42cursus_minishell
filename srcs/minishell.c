/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:16:37 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/09 17:30:16 by ji-kim           ###   ########.fr       */
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
			int		fd[2];
			int		prev_fd;
			int		fd_backup[2];

			tmp = ft_split(line, "|");
			total = 0;
			while (tmp[total])
				total++;
			if (total > 1 && PRINT)
				printf("total: %d\n", total);
			pid = malloc(sizeof(pid_t) * total);
			if (!pid)
				err_msg("malloc failed\n");
			if (total > 1)
			{
				fd_backup[0] = dup(STDIN_FILENO);
				fd_backup[1] = dup(STDOUT_FILENO);
			}
			for (int i = 0; i < total; i++)
			{
				/* child */
				pipe(fd);
				if (total > 1 && i < total - 1 && PRINT)
					printf("\t\t\t\e[34m--  fd[READ]: %d,  fd[WRITE]: %d\e[0m\n", fd[0], fd[1]);
				pid[i] = fork();
				if (pid[i] < 0)
					err_msg("fork failed\n");
				if (total > 1 && PRINT)
					printf("\e[31m====\t< FORK(%d) > : %d (%4d)\t====\n\e[0m", i, getpid(), getppid());
				if (pid[i] < 0)
					err_msg("fork failed\n");
				if (pid[i] == 0)
				{
					close(fd[0]);
					cmd = init_cmd(tmp[i], envl);
					if (!cmd || cmd->ret > 0)
						continue ;
					if (total > 1)
					{
						if (i > 0)
						{
							if (PRINT)
								printf("\e[34m-- IN    fd[%d] - child\e[0m\n", prev_fd);
							dup2(prev_fd, STDIN_FILENO);
							close(prev_fd);
						}
						if (i < total - 1)
						{
							if (PRINT)
								printf("\e[34m-- OUT   fd[%d] - child\e[0m\n", fd[1]);
							dup2(fd[1], STDOUT_FILENO);
							close(fd[1]);
						}
						else
						{
							if (PRINT)
								printf("\e[34m-- OUT   backup\e[0m\n");
							dup2(fd_backup[1], STDOUT_FILENO);
							close(fd_backup[1]);
						}
					}
					while (cmd)
					{
						if (total > 1 && PRINT)
							printf("\t -. %-6s /child/ :\t%d (%4d)\n", cmd->arg[0], getpid(), getppid());
						if (!ft_strcmp(cmd->arg[0], "echo"))
							builtin_echo(cmd);
						else if (!ft_strcmp(cmd->arg[0], "cd"))
						{
							write(fd[1], cmd->arg[1], (int)ft_strlen(cmd->arg[1]));
							exit(CMD_CD);
						}
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
							cmd = free_next(cmd);
							exit(CMD_EXIT);
						}
						else
							ft_exec(cmd, envl, total);
						if (PRINT)
							printf("\t< cmd >\t\t>> \e[1m%s\e[0m ends\n", cmd->arg[0]);
						cmd = free_next(cmd);
					}
					if (total > 1 && PRINT)
						printf("\t< cmd >\t\t>> child exit - (%d)\n", i);
					exit(EXIT_SUCCESS);
				}
				else
				{
					/* parent */
					int		status;
					waitpid(pid[i], &status, 0);
					close(fd[1]);
					if (total > 1 && PRINT)
						printf("\t -.        /parent/ :\t%d (%4d)\n", getpid(), getppid());
					if (WIFEXITED(status))
					{
						if (WEXITSTATUS(status) == CMD_EXIT)
						{
							// wait(0);
							printf("exit\n");
							exit(EXIT_SUCCESS);
						}
						else if (WEXITSTATUS(status) == CMD_CD)
						{
							char dir[100];

							read(fd[0], &dir, 99);
							builtin_cd(dir, &envl);
						}
						else
							g_ret = WEXITSTATUS(status);
					}
					if (PRINT)
						printf("\e[1;34m-- CLOSE fd[%d] - parent\e[0m\n", fd[1]);
					if (total > 1)
					{
						if (i > 0)
							close(prev_fd);
						prev_fd = fd[0];
					}
				}
				// close(fd[0]);
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
