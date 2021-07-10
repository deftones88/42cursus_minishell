#include "minishell.h"

extern int	g_ret;

void	cmd_loop(t_fd fd, t_cmd *cmd, t_list *envl, int total, struct termios t_old)
{
	while (cmd)
	{
		// PRINT start //
		if (total > 1 && PRINT)
			printf("\t -. %-6s /child/ :\t%d (%4d)\n", cmd->arg[0], getpid(), getppid());
		// PRINT end //

		if (!ft_strcmp(cmd->arg[0], "echo"))
			builtin_echo(cmd);
		else if (!ft_strcmp(cmd->arg[0], "cd"))
		{
			write(fd.fd[1], cmd->arg[1], (int)ft_strlen(cmd->arg[1]));
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

		// PRINT start //
		if (PRINT)
			printf("\t< cmd >\t\t>> \e[1m%s\e[0m ends\n", cmd->arg[0]);
		// PRINT end //
		
		cmd = free_next(cmd);
	}
}
