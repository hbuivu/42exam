#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int	ft_strlen(char *str)
{
	if (str == NULL)
		return (0); //?
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

void	(char *msg, char *arg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
	if (arg)
	{
		write (STDERR_FILENO, " ", 1);
		write(STDERR_FILENO, arg, ft_strlen(arg));
	}
	write(STDERR_FILENO, "\n", 1);
}

void ft_cd(int i, char **argv)
{
	if (i != 2)
		print_err("error: cd: bad arguments", NULL);
	else if (chdir(argv[1]) != 0)
		print_err("error: cd: cannot change directory to", argv[1]);
}

void ft_exec(int i, char **argv, char **envp)
{
	/* if ; or end
		pid = fork();
		if (pid == 0)
			dup2(something here);
			close (fd in dup2);
			execve(*argv, argv, envp);
			print_err("error: cannot execute", *argv);
			exit(1);
		else
			close(the one fd);
			waitpid for all the child processes
			tmpfd = dup(STDIN FILENO) //why??
	   if | 
	   	pipe(fd);
		pid = fork();
		if (pid == 0)
			close (fd[0]);
			dupe2(fd[1], STDOUT_FILENO) - fd[1] is the write end of pipe
			close (fd[1]);

	*/

	int fd[2];
	int pid;
	
	if (pipe(fd) == -1)
		return (print_err("error: fatal", NULL));
	pid = fork();
	if (pid == 0) //child
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1 || close(fd[0] == -1 || close(fd[1]) == -1))
			return (print_err("error: fatal", NULL));
		execve(*argv, argv, envp);
		return (print_err("error: cannot execute"), *argv);
	}
	waitpid(pid, NULL, 0);
	if (dup2(fd[0], STDIN_FILENO) == -1 || close(fd[0] == -1 || close(fd[1]) == -1))
		return (print_err("error: fatal", NULL));
	return (0);
}


int main(int argc, char** argv, char** envp)
{
	int i = 0;
	if (argc < 2)
		return (0);
	argv++;
	while (argv[i] && argv[i + 1])
	{
		argv += i;
		i = 0;
		while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
			i++;
		if (strcmp(argv[0], "cd") == 0)
			ft_cd(i, argv);
		else 
			ft_exec(i, argv, envp);
		//if i == 0 or we have ;, execute in stdout
		argv++
	}
	return (0);
}

// char **str = calloc(6, sizeof(char *));
	// str[0] = strdup("cd");
	// str[1] = strdup("home");
	// str[2] = strdup("|");
	// str[3] = strdup("cd");
	// str[4] = strdup("home");

	// while(str[i] && str[i + 1])
	// {
	// 	str += i;
	// 	printf("i: %i s: %s\n", i, *str);
	// 	i = 0;
	// 	while (str[i] && strcmp(str[i], "|") && strcmp(str[i], ";"))
	// 		i++;
	// 	str++;
	// }