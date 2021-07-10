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

			t_pid	pid;
			t_fd	fd;
			t_cmd	*cmd;

			init_pid(&pid, line);
			init_fd(&fd);
			for (int i = 0; i < pid.total; i++)
			{
				pipe(fd.fd);
				pid.pid[i] = fork();
				if (pid.pid[i] < 0)
					err_msg("fork failed\n");
				if (pid.pid[i] == 0)
				{
					/* child */
					cmd = init_cmd(pid.pipe_cmd[i], envl);
					if (!cmd || cmd->ret > 0)
						continue ;
					set_fd(&fd, pid.total, i);
					cmd_loop(fd, cmd, envl, pid.total, t_old);

					/* PRINT start */
					if (pid.total > 1 && PRINT)
						printf("\t< cmd >\t\t>> child exit - (%d)\n", i);
					/* PRINT end */

					exit(EXIT_SUCCESS);
				}
				else
				{
					/* parent */
					int		status;

					waitpid(pid.pid[i], &status, 0);
					close(fd.fd[1]);
					exit_status(fd, status, envl);
					if (pid.total > 1)
					{
						if (i > 0)
							close(fd.prev_fd);
						fd.prev_fd = fd.fd[0];
					}
				}
			}
			if (PRINT)
				printf("\e[1;34m-- CLOSE fd[%d] - parent\e[0m\n", fd.fd[1]);
			close(fd.prev_fd);
			if (pid.total > 1)
			{
				if (PRINT)
					printf("\e[34m-- IN backup\e[0m\n");
				dup_close(fd.fd_bu[0], STDIN_FILENO);
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
