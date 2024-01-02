#include <stdio.h>
void ft_cd(char *arg)
{
	if (chdir(arg) != 0)
		print_fd("error: cd: cannot change directory to", arg, "\n");
}
int main(int argc, char** argv, char** envp)
{
	
}