#include "minishell.h"

extern int	g_ret;

void	cmd_loop(t_fd fd, t_cmd *cmd, t_list *envl, int total, struct termios t_old)
{
	while (cmd)
	{
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
			ft_exec(cmd, envl);
		cmd = free_next(cmd);
	}
}
