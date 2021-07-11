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

int	g_ret = 0;

void	show_logo(void)
{
	printf("\n-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-=x=-\n");
	printf("           ╔╗        ╔╗╔╗                     _.|  '\n");
	printf("          ╔╬╣ ╔═╦╦╦╗ ║╠╬╬══╗                .'  | /\n");
	printf("          ╠╣╠═╚╩═╩═╝═╣═╣║║║║             ,'     |'\n");
	printf("         ╔╝╠╝        ╚╩╩╩╩╩╝            /      /\n");
	printf("         ╚═╝              _..----\"\"---.'      /\n");
	printf("    _.....---------...,-\"\"                  ,'\n");
	printf("    `-._  \\                                /\n");
	printf("        `-.+_            __           ,xx. .\n");
	printf("             `-.._     .:##).        (`**\"| \\\n");
	printf("                  7    | `\" |         `...'  \\\n");
	printf("                  |     `--'     '+\"        ,\". ,\"\"-\n");
	printf("                  |   _...        .____     | |/    '\n");
	printf("             _.._ |  .    `.  '--\"   /      `./     j\n");
	printf("            '    `-. `---\"    `.    /        /     /\n");
	printf("  __^__      \\       `.          `-\"     _,'      /       __^__\n");
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
	// t_all	*all;
	t_pid	pid;
	t_fd	fd;
	struct termios	t_old;
	t_list	*envl;
	t_cmd	*cmd;

	// all = ft_calloc(sizeof(t_all), 1);
	// if (!all)
	// 	err_msg("malloc failed\n");
	fd.fd_bu[0] = dup(STDIN_FILENO);
	fd.fd_bu[1] = dup(STDOUT_FILENO);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
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
			pid.pipe_cmd = ft_split(line, "|");
			pid.total = 0;
			while (pid.pipe_cmd[pid.total])
				pid.total++;
			pid.pid = malloc(sizeof(pid_t) * pid.total);
			if (!pid.pid)
				err_msg("malloc failed\n");
			for (int i = 0; i < pid.total; i++)
			{
				pipe(fd.fd);
				pid.pid[i] = fork();
				if (pid.pid[i] < 0)
					err_msg("fork failed\n");
				if (pid.pid[i] == 0)
				{
					cmd = init_cmd(pid.pipe_cmd[i], envl);
					if (!cmd || cmd->ret > 0)
						continue ;
					set_fd(&fd, pid.total - 1, i);
					while (cmd)
					{
						if (!ft_strcmp(cmd->arg[0], "echo"))
							builtin_echo(cmd);
						else if (!ft_strcmp(cmd->arg[0], "cd"))
							cd_pipe(fd.fd[1], cmd->arg[1]);
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
							exit(CMD_EXIT);
						else
							ft_exec(cmd, envl);
						cmd = free_next(cmd);
					}
					exit(EXIT_SUCCESS);
				}
				else
				{
					int		status;

					waitpid(pid.pid[i], &status, 0);
					close(fd.fd[1]);

					if (WIFEXITED(status))
					{
						if (WEXITSTATUS(status) == CMD_EXIT)
						{
							printf("exit\n");
							tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
							exit(EXIT_SUCCESS);
						}
						else if (WEXITSTATUS(status) == CMD_CD)
						{
							char	dir[100];
							read(fd.fd[0], &dir, 99);
							builtin_cd(dir, &envl);
						}
						else
							g_ret = WEXITSTATUS(status);
					}
					if (pid.total > 1)
					{
						if (i > 0)
							close(fd.prev_fd);
						fd.prev_fd = fd.fd[0];
					}
				}
			}
			close(fd.prev_fd);
			if (pid.total > 1)
				dup_close(fd.fd_bu[0], STDIN_FILENO);
			// free(pid.pid);
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
	return (0);
}
