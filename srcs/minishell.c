#include "minishell.h"

void show_logo(void)
{
	printf("                    ╔╗  ╔╦═╦╗ \n");
	printf("                 ╔══╬╬═╦╬╣═╣╚╦═╦╗╦╗ \n");
	printf("  ╬══════════════║║║║║║║║╠═║║║╩╣╚╣╚╗══════════════╬  \n");
	printf("  ╬══════════════╚╩╩╩╩╩═╩╩═╩╩╩═╩═╝═╝══════════════╬  \n");
	printf("  ║                                               ║  \n");
	printf("  ║    ▒ ▒            ██████████            ▒ ▒   ║  \n");
	printf("  ║     ▒          ███          ██           ▒    ║  \n");
//	printf("  ║               ██             ██               ║  \n");
//	printf("  ║               █               ██      ▀       ║  \n");
	printf("  ║               █                █              ║  \n");
	printf("  ║              ██   █        █   ██             ║  \n");
	printf("  ║     ▒ ▒      █     ███████      █    ▒ ▒      ║  \n");
//	printf("  ║             █                   █             ║  \n");
	printf("  ║      ▒      ██                  █     ▒       ║  \n");
	//printf("  ║              ███               ██             ║  \n");
	printf("  ║╔╗    ╔╗╔╗      ████████████████     ╔╗  ╔╗╔╗  ║  \n");
	printf("  ╔╬╬═╦╦╦╣╠╬╬══╗      █        █       ╔╬╬══╣╠╬╬══╗  \n");
	printf("  ╠╣║║║║║║═╣║║║║                       ╠╣╠══╣═╣║║║║  \n");
	printf(" ╔╝╠╩╩═╩═╩╩╩╩╩╩╝══════════════════════╔╝╠╝══╚╩╩╩╩╩╝  \n");
	printf(" ╚═╝══════════════════════════════════╚═╝═════════╬  \n");
}

int		main(int argc, char **argv, char **envp)
{
	char	*line;
	char	*dir;
	t_cmd	*cmd;
	t_list	*envl;

	/* ctrl commands */
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);

	/* termios for muting "^C" */
	struct termios	t_old;
	tcgetattr(STDIN_FILENO, &t_old);
	set_term();

	envl = init_env(envp);
	show_logo();

	while(1)
	{
		dir = ft_strjoin(ft_strrchr(getcwd(NULL, 0), '/') + 1, "$ ");
		line = readline(dir);
		if (line && line[0] != 0)
		{
			add_history(line);
			cmd = init_cmd(line, envl);
			while (cmd)
			{
				if (!ft_strcmp(cmd->arg[0], "echo"))
					builtin_echo(cmd);
				else if (!ft_strcmp(cmd->arg[0], "cd"))
					cmd->ret = chdir(cmd->arg[1]);
				else if (!ft_strcmp(cmd->arg[0], "pwd"))
					printf("%s\n", getcwd(NULL, 0));
				else if (!ft_strcmp(cmd->arg[0], "export"))
					builtin_export(cmd, &envl);
				else if (!ft_strcmp(cmd->arg[0], "unset"))
					builtin_unset(cmd, &envl);
				else if (!ft_strcmp(cmd->arg[0], "env"))
					builtin_env(cmd, envl, 0);
				else if (!ft_strcmp(cmd->arg[0], "exit"))
				{
					tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
					exit(EXIT_SUCCESS);
				}
				else
					ft_exec(cmd, envl);
				cmd = free_next(cmd);
			}
		}
		else if (line == NULL)
		{
			tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
			exit(EXIT_SUCCESS);
		}
		free(line);
		free(dir);
	}
}
