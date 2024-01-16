#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int ft_strlen(char *str)
{
	if (str == NULL)
		return (0);
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

int	error(char *msg, char *arg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
	if (arg)
	{
		write(STDERR_FILENO, " ", 1);
		write(STDERR_FILENO, arg, ft_strlen(arg));
	}
	write(STDERR_FILENO, "\n", 1);
	return (1);
}

void	ft_cd(int i, char **argv)
{
	if (i != 2)
		error("error: cd: bad arguments", NULL);
	else if (chdir(argv[1]) == -1)
		error("error: cd: cannot change directory to", argv[1]);
}

int main(int argc, char **argv, char **envp)
{
	int fd[2];
	int status;
	int pid;
	int prev_fd = dup(STDIN_FILENO);
	int i = 0;

	if (argc < 2)
		return (0);
	while (argv[i] && argv[i + 1])
	{
		argv = argv + i + 1;
		i = 0;
		while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
			i++;
		if (i != 0 && strcmp(argv[0], "cd") == 0)
			ft_cd(i, argv);
		else if (i != 0 && (argv[i] == NULL || strcmp(argv[i], ";") == 0))
		{
			pid = fork();
			if (pid == -1)
				return (error("error: fatal", NULL));
			if (pid == 0)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1 || close(prev_fd) == -1)
					return (error("error: fatal", NULL));
				argv[i] = NULL;
				execve(argv[0], argv, envp);
				error("error: cannot execute", argv[0]);
				exit(1);
			}
			if (close(prev_fd) == -1)
				return (error("error: fatal", NULL));
			prev_fd = dup(STDIN_FILENO);
			if (prev_fd == -1)
				return (error("error: fatal", NULL));
			waitpid(pid, &status, 0);
		}
		else if (i != 0 && strcmp(argv[i], "|") == 0)
		{
			if (pipe(fd) == -1)
				return (error("error: fatal", NULL));
			pid = fork();
			if (pid == -1)
				return (error("error: fatal", NULL));
			if (pid == 0)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1 || dup2(fd[1], STDOUT_FILENO) == -1 ||
					close(fd[0]) == -1 || close(fd[1]) == -1 || close(prev_fd) == -1)
					return (error("error: fatal", NULL));
				argv[i] = NULL;
				execve(argv[0], argv, envp);
				error("error: cannot execute", argv[0]);
				exit(1);
			}
			if (close(fd[1]) == -1 || close(prev_fd) == -1)
				return (error("error: fatal", NULL));
			prev_fd = fd[0];
			waitpid(pid, &status, 0);
		}
	}

}