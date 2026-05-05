#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int	picoshell(char **cmds[])
{
	int	i = 0;
	int	prev = -1;
	int	fd[2];
	int	err = 0;
	int	pid;
	int	last;
	int	status;

	if (!cmds)
		return (0);

	while (cmds[i] && !err)
	{
		last = !cmds[i + 1];
		if (!last && pipe(fd) < 0) { err = 1; break ; }
		pid = fork();
		if (pid < 0)
		{
			if (!last) { close(fd[0]); close(fd[1]); }
			err = 1;
			break ;
		}
		if (!pid)
		{
			if (prev >= 0)
			{
				if (dup2(prev, 0) < 0)
					exit(1);
				close(prev);
			}
			if (!last)
			{
				close(fd[0]);
				if (dup2(fd[1], 1) < 0)
					exit(1);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (prev >= 0) close(prev);
		if (!last) { close(fd[1]); prev = fd[0]; }
		else prev = -1;
		i++;
	}
	if (prev >= 0) close(prev);
	while (wait(&status) > 0)
	{
		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
			err = 1;
	}
	return (err);
}
