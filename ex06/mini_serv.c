#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

//each client will have an id as specified by subject
//each client will have a mailbox to store their messages until sent
typedef struct client
{
	//i guess mailbox should be dynamically set up too :(
	char mailbox[100000];
	int id;

} client;

//error function as specified by subject
void	ret_err()
{
	write(2, "Fatal Error\n", 11);
	exit(1);
}

//Check port and return port integer
//port needs to be in between 1024 and 65535
int	check_port(char *port_arg)
{
	int len = strlen(port_arg);
	
	//if len is < 4 or > 5, then it does not fall in between the allowed ports
	if (len < 4 || len > 5)
		ret_err();
	
	//port must be integer only
	for (int i = 0; i < len; i++)
		if (port_arg[i] < '0' || port_arg[i] > '9')
			ret_err();
	
	//port must be in between 1024 and 65535
	int port = atoi(port_arg);
	if (port < 1024 || port > 65535)
		ret_err();
	
	return (port);
}

// create listening socket for the server and return the socket fd
// int socket(int domain, int type, int protocol);
// int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
// struct sockaddr_in {
//     short int          sin_family;  // Address family, AF_INET
//     unsigned short int sin_port;    // Port number
//     struct in_addr     sin_addr;    // Internet address
//     unsigned char      sin_zero[8]; // Same size as struct sockaddr
// };
// struct in_addr {
// 	uint32_t s_addr; // that's a 32-bit int (4 bytes)
// };

int	create_listen_socket(int port)
{
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
		ret_err();

	// set up sockaddr_in to use in bind
	// INADDR_ANY - socket should bind to all available network interfaces
	// INADDR_LOOPBACK - for localhost communication only
	// htons = host to network short
	// htonl = host to network long
	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind socket to port
	if (bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		ret_err();
	
	//set up socket as a listening socket
	if (listen(server_socket, 20) == -1)
		ret_err();
	
	return (server_socket);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(2, "Wrong number of arguments\n", 27);
		exit(1);
	}
	//convert port
	int port = check_port(argv[1]);
	printf("port: %i\n", port);

	// create listening socket
	int server_socket = create_listen_socket(port);
	printf("server_socket: %i\n", server_socket);

	//set up an array of clients dynamically?
	client client_array[1024];
	memset(client_array, 0, sizeof(client_array));

	// //set up client id;
	// int next_client_id = 1;
	
	// create fd sets and initialize to use with select() function;
	// void FD_ZERO(fd_set *set); -> macro from select() function to initialize fd set to empty
	fd_set readfds, writefds, currfds;
	FD_ZERO(&currfds); //keep track of all our active fds
	FD_ZERO(&readfds); //any fds that are free to be read from will be dropped in this fd set by select
	FD_ZERO(&writefds); //any fds that are free to be written to will be dropped in this fd set by select
	
	// // add server_socket to currfds;
	// // void FD_SET(int fd, fd_set *set); -> add an fd to a specified fd set
	// FD_SET(server_socket, &currfds);

	// //initialize maxfd to be used in select();
	// int maxfd = server_socket;
	
	// //set up server loop
	// while (1)
	// {
	// 	// copy fds from currfds to readfds and writefds
	// 	readfds = currfds;
	// 	writefds = currfds;

	// 	// int select(int numfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout); 
	// 	// select checks all fds from the maxfd, and will put any readable fds into readfds and any writable fds into writefds
	// 	// numfds should be set to highest numbered fd in any set plus 1
	// 	int ret = select(maxfd + 1, &readfds, &writefds, NULL, NULL);
		
	// 	//if select errors out, close all fds and exit program
	// 	if (ret < 0)
	// 	{
	// 		//close all fds
	// 		for (int fd = 0; fd <= maxfd; fd++)
	// 			if (client_array[fd].id != 0)
	// 				close(fd);
	// 		ret_err();
	// 	}
		
	// 	// iterate over all fds in currfds
	// 	// int FD_ISSET(int fd, fd_set *set); -> checks if fd is part of specified fd set
	// 	for (int fd = 0; fd <= maxfd; fd++)
	// 	{
	// 		//if server_socket is readable, add new client
	// 		if (FD_ISSET(server_socket, &readfds))
	// 		{
	// 			//get new client fd
	// 			socklen_t addrlen = sizeof(struct sockaddr_in);
	// 			struct sockaddr_in client_info;
	// 			bzero(&client_info, sizeof(struct sockaddr_in));
	// 			int newfd = accept(server_socket, (struct sockaddr *)&client_info, &addrlen);
	// 			if (newfd == -1)
	// 				continue; //??correct??

	// 			//set up client struct in client_array
	// 			client_array[newfd].id = next_client_id;
				
	// 			//prep next_client_id for next client;
	// 			next_client_id++;
				
	// 			//add new fd to currfds list
	// 			FD_SET(newfd, &currfds);

	// 			//drop arrival message in existing client's mailboxes
	// 			char msg_buffer[50];
	// 			bzero(msg_buffer, sizeof(msg_buffer));
	// 			sprintf(msg_buffer, "server: client %d just arrived\n", client_array[newfd].id); 
	// 			for (int fd; fd <= maxfd; fd++)
	// 				if (client_array[fd].id != 0)
	// 					strcat(client_array[newfd].mailbox, msg_buffer);

	// 			//set new maxfd
	// 			if (newfd > maxfd)
	// 				maxfd = newfd;

	// 		}

	// 		//if client fd can be read from, read message sent from client to server
	// 		else if (FD_ISSET(fd, &readfds))
	// 		{
	// 			//ssize_t recv(int sockfd, void *buf, size_t len, int flags);
	// 			char buf[150001];
	// 			bzero(buf, 150001);
	// 			int read_bytes = recv(fd, buf, 150000, 0);
	// 			if (read_bytes == 0)
	// 			{
	// 				// delete fd from fd set currfds
	// 				FD_CLR(fd, &currfds);
					
	// 				//prep leaving message
	// 				char msg_buffer[50];
	// 				bzero(msg_buffer, sizeof(msg_buffer));
	// 				sprintf(msg_buffer, "server: client %d just left", client_array[fd].id);
					
	// 				//delete client from client_arrays
	// 				client_array[fd].id = 0;

	// 				//drop message in remaining clients' mailboxes
	// 				for (int fd; fd <= maxfd; fd++)
	// 					if (client_array[fd].id != 0)
	// 						strcat(client_array[fd].mailbox, msg_buffer);

	// 				//close fd
	// 				close(fd);
	// 				continue;
	// 			}
	// 			//if there's a message
	// 			else if (read_bytes > 0)
	// 			{
	// 				//download and format message
	// 				//format client %d after every newline?? REDO
	// 				char msg_buffer[read_bytes + 1];
	// 				bzero(msg_buffer, sizeof(msg_buffer));
	// 				sprintf(msg_buffer, "client %d: %s", client_array[fd].id, msg_buffer);

	// 				//drop message in remaining clients' mailboxes
	// 				for (int fd; fd <= maxfd; fd++)
	// 					if (client_array[fd].id != 0)
	// 						strcat(client_array[fd].mailbox, msg_buffer);
	// 			}
	// 		}
			
	// 		// if fd can be written to, send all messages from client mailbox
	// 		// ssize_t send(int sockfd, const void *buf, size_t len, int flags);
	// 		else if (FD_ISSET(fd, &writefds))
	// 		{
	// 			//might have to send using getnextline -> can't send a message wtihout a newline?
	// 			if (send(fd, client_array[fd].mailbox, sizeof(client_array[fd].mailbox), 0) == -1)
	// 				continue; //do we need to write an error here?
				
	// 			//clear out mailbox up until newline?
	// 		}
	// 	}
	// }
	return (0);
	
}
