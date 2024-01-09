#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

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
			ft_cd(argv);
		else if (num_args != 0 && (argv[num_args] == NULL || strcmp(argv[num_args], ";") == 0))
		{
			pid = fork();
			if (pid == 0)
			{
				argv[num_args] = NULL;
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
				execve(argv[0], argv, envp);
				error();
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
}