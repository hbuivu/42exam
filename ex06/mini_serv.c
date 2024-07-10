#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void	ret_err()
{
	write (2, "Fatal Error\n", 11);
	exit(1);
}

int	create_listen_socket(int port)
{
	//create listening socket
	// int socket(int domain, int type, int protocol); 
	int listen_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock_fd) == -1
		ret_err();

	//bind listening socket tor port
	//int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
	// struct sockaddr_in {
	//     short int          sin_family;  // Address family, AF_INET
	//     unsigned short int sin_port;    // Port number
	//     struct in_addr     sin_addr;    // Internet address
	//     unsigned char      sin_zero[8]; // Same size as struct sockaddr
	// };
	// struct in_addr {
	// 	uint32_t s_addr; // that's a 32-bit int (4 bytes)
	// };
	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY)
	//can use INADDR_LOOPBACK - for localhost communication only
	//INADDR_ANY - socket should bind to all available network interfaces
	//can we use htonl?

	//bind socket to port
	if (bind(listen_sock_fd, &serv_addr, sizeof(serv_addr)) == -1)
		ret_err();
	
	//set up socket as a listening socket
	if (listen(listen_sock_fd, 10) == -1)
		ret_err();
	
	return (listen_sock_fd);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Wrong number of argument\n");
		exit(1);
	}

	//check port
	//port needs to be in between 1024 and 65535
	//port needs to be an integer only
	//convert port to an int
	int port = atoi(argv[1]);

	//create listening socket
	int listen_sock_fd = create_listen_socket(port);
	
	//set up server loop
	int run = 1;
	while (run == 1)
	{
		//poll the fds using select
		// int select(int numfds, fd_set *readfds, fd_set *writefds,
        //    fd_set *exceptfds, struct timeval *timeout); 
		// select checks a set of fds and 
		// numfds should be set to highest numbered fd in any set plus 1
		// void FD_ZERO(fd_set *set); -> initialize fd set to empty 
		// void FD_SET(int fd, fd_set *set); -> tell select that you want to monitor this fd
		// int FD_ISSET(int fd, fd_set *set); -> checks if fd is part of specified fd set
		fd_set	readfds, writefds, currfds;
		FD_ZERO(readfds);
		FD_ZERO(writefds);
		FD_SET(listen_sock_fd, &list_fds);
		int maxfd = client_fd; //????
		int ret = select (num_fds, &fd_list, NULL, NULL, NULL);
		if (ret < 0)
		{
			//close all fds
			ret_err();
		}

		//iterate over each fd and check revents
			//if socket is listen socket, add new client
			//if revents is POLLIN read msg
			//if revents is POLLOUT send msg
			//if revents is POLLHUP delete fd
	}
	

	



	

	
	
	//set up server struct with all the info
	//functions to use: socket, bind
	//poll all fds



	
}

// 	1. create listening socket
// In while loop
// 2. set a switch int change - tracks if any fds have been added to array
// 3. poll all fds - this function will switch revents (in struct) if fd is ready for some kind of action
// 4. iterate through every fd and check if there is POLLIN/POLLOUT/POLLHUP in revents
// 	POLLIN - data can be read from fd without blocking
// 	POLLOUT - data can be written to fd without blocking
// 	POLLHUP - fd has been closed or disconnected - delete it from our array
// 5. if a new client has been added, modify our array accordingly
// 7. free addrinfo struct (and everything else that needs to be freed)

// struct addrinfo {
//     int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
//     int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC
//     int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
//     int              ai_protocol;  // use 0 for "any"
//     size_t           ai_addrlen;   // size of ai_addr in bytes
//     struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
//     char            *ai_canonname; // full canonical hostname

//     struct addrinfo *ai_next;      // linked list, next node
// };