#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static pid_t		g_pid;
static volatile int	g_timeout;

static void	alrm_handler(int sig)
{
	(void)sig;
	g_timeout = 1;
	kill(g_pid, SIGKILL);
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	int				status;
	struct sigaction	sa;

	g_timeout = 0;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = alrm_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGALRM, &sa, NULL);
	g_pid = fork();
	if (g_pid < 0)
		return (-1);
	if (!g_pid)
	{
		f();
		exit(0);
	}
	alarm(timeout);
	waitpid(g_pid, &status, WUNTRACED);
	alarm(0);
	if (g_timeout)
	{
		if (verbose) printf("Bad function: timed out after %u seconds\n", timeout);
		return (0);
	}
	if (WIFSTOPPED(status))
	{
		kill(g_pid, SIGKILL);
		waitpid(g_pid, NULL, 0);
		if (verbose) printf("Bad function: %s\n", strsignal(WSTOPSIG(status)));
		return (0);
	}
	if (WIFEXITED(status))
	{
		int	code = WEXITSTATUS(status);
		if (!code) { if (verbose) printf("Nice function!\n"); return (1); }
		if (verbose) printf("Bad function: exited with code %d\n", code);
		return (0);
	}
	if (WIFSIGNALED(status))
	{
		if (verbose) printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
		return (0);
	}
	return (-1);
}
