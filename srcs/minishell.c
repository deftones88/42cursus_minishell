#include "minishell.h"

int		main(int argc, char **argv, char **envp)
{
	char	*line;
	char	*dir;
	t_list	*history;
	t_cmd	cmd;

	setbuf(stdout, NULL);
	history = 0;
	dir = "current dir";
	while(1)
	{
		printf("%s$ ", dir);
		if (get_next_line(STDIN, &line) != 1 || !line)
		{
			if (line)
				free(line);
			break ;
		}
		ft_lstadd_front(&history, ft_lstnew(line));
		printf("->%s\n",line);
		free(line);
	}
}
