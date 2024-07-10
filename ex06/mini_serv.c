#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// struct sockaddr_in {
//     short int          sin_family;  // Address family, AF_INET
//     unsigned short int sin_port;    // Port number
//     struct in_addr     sin_addr;    // Internet address
//     unsigned char      sin_zero[8]; // Same size as struct sockaddr
// };

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
	int port = atoi(argv[1]);

	//create listening socket
	// int socket(int domain, int type, int protocol); 
	int listen_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock_fd) == -1
	{
		write (2, "Fatal Error\n", 11);
		exit(1);
	}

	//bind listening socke tor port
	//int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = port;
	serv_addr.sin_addr = 

	if (bind(listen_sock_fd, &serv_addr, sizeof(addr)) == -1)
	{
		write (2, "Fatal Error\n", 11);
		exit(1);
	}


	

	
	
	//set up server struct with all the info
	//functions to use: socket, bind

	hints.ai_family = AF_INET; //IPV4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, port, &hints, &server) != 0 || 
		(listenSockfd = socket(server->ai_family, server->ai_socktype, server->ai_protocol)) == -1 ||
		setsockopt(listenSockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 ||
		bind(listenSockfd, server->ai_addr, server->ai_addrlen) == -1 ||
		listen(listenSockfd, 10) == -1)
	{
		write(2, "Fatal Error\n", 11);
		exit (1);
	}
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