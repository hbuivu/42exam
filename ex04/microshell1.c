#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int ft_strlen(char *str)
{
	if (str == NULL)
		return 0;
	int i = 0;
	while(str[i])
		i++;
	return (i);
}

void	error(char *msg, char *arg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
	if (arg)
	{
		write(STDERR_FILENO, " ", 1);
		write(STDERR_FILENO, arg, ft_strlen(arg));
	}
	write (STDERR_FILENO, "\n", 1);
}

void ft_cd(int num_args, char **argv)
{
	if (num_args != 2)
		error("error: cd: bad arguments", NULL);
	else if (chdir(argv[1]) == -1)
		error("error: cd: cannot change directory to", argv[1]);
}

int main(int argc, char **argv, char **envp)
{
	int fd[2];
	int status;
	int pid;
	int num_args = 0;
	int prev_fd = dup(STDIN_FILENO);

	if (argc < 2)
		return (0);
	while (argv[num_args] && argv[num_args + 1])
	{
		argv = argv + num_args + 1;
		num_args = 0;
		while (argv[num_args] && strcmp(argv[num_args], "|") && strcmp(argv[num_args], ";"))
			num_args++;
		if (num_args != 0 && strcmp(argv[num_args], "cd") == 0)
			ft_cd(num_args, argv);
		else if (num_args != 0 && (argv[num_args] == NULL || strcmp(argv[num_args], ";") == 0))
		{
			pid = fork();
			if (pid == 0)
			{
				argv[num_args] = NULL;
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
				execve(argv[0], argv, envp);
				error("error: cannot execute", argv[0]);
				exit(1);
			}
			close(prev_fd);
			prev_fd = dup(STDIN_FILENO);
			waitpid(pid, &status, 0);
		}
		else if (num_args != 0 && strcmp(argv[num_args], "|") == 0)
		{
			pipe(fd);
			pid = fork();
			if (pid == 0)
			{
				argv[num_args] == NULL;
				dup2(prev_fd, STDIN_FILENO);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				close(prev_fd);
				execve(argv[0], argv, envp);
				error();
				exit(1);
			}
			close(prev_fd);
			close(fd[1]);
			prev_fd = fd[0];
			waitpid(pid, &status, 0);
		}
	}
	return (0);
}