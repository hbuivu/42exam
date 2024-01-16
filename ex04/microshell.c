#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int	ft_strlen(char *str)
{
	if (str == NULL)
		return (0);
	int num_args = 0;
	while (str[num_args])
		num_args++;
	return (num_args);
}

void	print_err(char *msg, char *arg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
	if (arg)
	{
		write (STDERR_FILENO, " ", 1);
		write(STDERR_FILENO, arg, ft_strlen(arg));
	}
	write(STDERR_FILENO, "\n", 1);
}

void ft_cd(int num_args, char **argv)
{
	if (num_args != 2)
		print_err("error: cd: bad arguments", NULL);
	else if (chdir(argv[1]) != 0)
		print_err("error: cd: cannot change directory to", argv[1]);
}

int main(int argc, char** argv, char** envp)
{
	int num_args = 0;
	int prev_fd = dup(STDIN_FILENO);
	int fd[2];
	int status;
	int pid;

	if (argc < 2)
		return (0);
	while (argv[num_args] && argv[num_args + 1])
	{
		argv = argv + num_args + 1;
		num_args = 0;
		while (argv[num_args] && strcmp(argv[num_args], "|") && strcmp(argv[num_args], ";"))
			num_args++;
		if (strcmp(argv[0], "cd") == 0)
			ft_cd(num_args, argv);
		else if (num_args != 0 && (argv[num_args] == NULL || strcmp(argv[num_args], ";") == 0)) //if last argument or ;
		{
			pid = fork();
			if (pid == 0)
			{
				argv[num_args] = NULL;
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
				execve(argv[0], argv, envp);
				print_err("error: cannot execute", argv[0]);
				exit(1);
			}
			close(prev_fd);
			if (argv[num_args] != NULL)
				prev_fd = dup(STDIN_FILENO);
			waitpid(pid, &status, 0);
		}
		else if (num_args != 0 && strcmp(argv[num_args], "|") == 0) //if pipe
		{
			pipe(fd);
			pid = fork(); 
			if (pid == 0)
			{
				argv[num_args] = NULL;
				dup2(fd[1], STDOUT_FILENO);
				dup2(prev_fd, STDIN_FILENO);
				close(fd[0]);
				close(fd[1]);
				close(prev_fd);
				execve(argv[0], argv, envp);
				print_err("error: cannot execute", argv[0]);
				exit(1);
			}
			close(fd[1]);
			close(prev_fd);
			prev_fd = fd[0]; //save read end for the next command 
			waitpid(pid, &status, 0);
		}
	}
	return (0);
}

/* 
If cd
	follow cd
If last argument
	read from previous fd
	execute and send to stdout
if argument is followed by ;
	read from previous fd
	execute and send to stdout
if argument is followed by |
	read from previous fd
	write to the next pipe
	execute and send read end of next pipe
if first argument
	write to next pipe
	execute and send to read end of next pipe

CHILD PROCESS
-terminate where pipe is in argv to NULL so it can be sent through execve
-dup necessary fds
-close all fds
-execute
-print error message if error
-exit 1

PARENT PROCESS
close necessary fds
dup previous fd
waitpid
*/
	