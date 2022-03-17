#include "../includes/libhmreadline.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


char *get_cwd()
{
	return ("Coucou");
}

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int main(void)
{
	int fd_history;
	char *cmd;

	fd_history = open("/home/menzo/.minishell_history", O_RDWR | O_APPEND | O_CREAT, 0777);
	while (1)
	{
		cmd = xreadline(&get_cwd, fd_history);
		if (!cmd || !*cmd)
			break;
		write(fd_history, cmd, ft_strlen(cmd));
		write(fd_history, "\n", 1);
	}
	xfree_all();
	return 0;
}
