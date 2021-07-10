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
	t_all	*all;

	all = init_all(envp);
	show_logo();
	while(1)
	{
		line = readline("minishell$ ");
		if (line && line[0] != 0)
		{
			add_history(line);
			init_pid(&all->pid, line);
			pid_loop(all);
		}
		else if (line == NULL)
		{
			set_termcap(0);
			printf("minishell$ exit\n");
			tcsetattr(STDIN_FILENO, TCSANOW, &all->t_old);
			exit(EXIT_SUCCESS);
		}
		free(line);
	}
	return (0);
}
