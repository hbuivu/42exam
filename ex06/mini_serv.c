#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

//set global variable to keep track of next client id
int next_client_id = 1;

//set global variable to keep track of maxfd
int maxfd = 0;

//each client will have an id as specified by subject
//each client will have a mailbox to store their messages until sent
typedef struct client
{
	char *mailbox;
	int id;

} client;

enum tags
{
	NEWCLIENT,
	CLIENTLEFT,
	NEWMESSAGE
};

void	delete_client(int fd, client *clientlist)
{
	if (clientlist[fd].mailbox != NULL)
	{
		free(clientlist[fd].mailbox);
		clientlist[fd].mailbox = NULL;
	}
	clientlist[fd].id = 0;
	close(fd);
}

void	exit_program(client *clientlist)
{
	if (clientlist != NULL)
	{
		for (int fd = 0; fd <= maxfd; fd++)
			if (clientlist[fd].id != 0)
				delete_client(fd, clientlist);
		free(clientlist);
	}
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
		exit_program(NULL);
	
	//port must be integer only
	for (int i = 0; i < len; i++)
		if (port_arg[i] < '0' || port_arg[i] > '9')
			exit_program(NULL);
	
	//port must be in between 1024 and 65535
	int port = atoi(port_arg);
	if (port < 1024 || port > 65535)
		exit_program(NULL);
	
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
		exit_program(NULL);

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
		exit_program(NULL);
	
	//set up socket as a listening socket
	if (listen(server_socket, 20) == -1)
		exit_program(NULL);
	
	return (server_socket);
}

char	*generate_msg(int id, char *msg, int tag)
{
	char *buf;
	if (tag == NEWCLIENT)
	{
		buf = (char *)calloc(50, sizeof(char));
		sprintf(buf, "server: client %d just arrived\n", id);
	}
	else if (tag == CLIENTLEFT)
	{
		buf = (char *)calloc(50, sizeof(char));
		sprintf(buf, "server: client %d just left\n", id);
	}
	else if (tag == NEWMESSAGE)
	{
		buf = (char *)calloc(50, sizeof(char));
		// sprintf(msg_buffer, "client %d: %s", clientlist[fd].id, msg_buffer);
		sprintf(buf, "client %d: new message\n", id);
	}
	return (buf);
}

//deliver msg to client mailbox - not actually sending message!!
void	deliver_msg(char *msg, client *clientlist)
{
	if (clientlist != NULL)
	{
		for (int fd = 0; fd <= maxfd; fd++)
		{
			if (clientlist[fd].id != 0)
			{
				int msg_len = strlen(msg);
				int mailbox_len = strlen(clientlist[fd].mailbox);
				char *temp = (char *)realloc(clientlist[fd].mailbox, msg_len + mailbox_len + 1);
				if (temp == NULL)
					exit_program(clientlist);
				strcat(clientlist[fd].mailbox, msg);
			}
		}
	}
	free(msg);
}

//send message if client is available to be written to
void	send_msg(int fd, client *clientlist)
{
	// ssize_t send(int sockfd, const void *buf, size_t len, int flags);
	if (send(fd, clientlist[fd].mailbox, sizeof(clientlist[fd].mailbox), 0) == -1)
		return; //do we need to write an error here?
	free(clientlist[fd].mailbox);
	clientlist[fd].mailbox = NULL;
}

void	add_new_client(int server_socket, client *clientlist, fd_set *currfds)
{
		//get new client fd
		socklen_t addrlen = sizeof(struct sockaddr_in);
		struct sockaddr_in client_info;
		memset(&client_info, 0, sizeof(struct sockaddr_in));
		int newfd = accept(server_socket, (struct sockaddr *)&client_info, &addrlen);
		if (newfd == -1)
			return ;

		//drop arrival message in existing client's mailboxes
		if (clientlist != NULL)
		{
			char *msg = generate_msg(next_client_id, NULL, NEWCLIENT);
			deliver_msg(msg, clientlist);
		}
		
		//check maxfd to see if we need to reset it and resize clientlist
		if (newfd > maxfd)
		{
			maxfd = newfd;
			//if this is the first instanstiaion of clientlist
			if (clientlist == NULL)
				clientlist = (client *)calloc(maxfd + 1, sizeof(client));
			else
			{
				client *temp = (client *)realloc(clientlist, maxfd + 1);
				if (temp == NULL)
					exit_program(clientlist);
				clientlist = temp;
			}
		}

		//set up new client struct in clientlist
		clientlist[newfd].id = next_client_id;
		clientlist[newfd].mailbox = NULL;
		
		//prep next_client_id for next client;
		next_client_id++;
		
		//add new fd to currfds list
		FD_SET(newfd, currfds);

}

void	read_msg(int fd, client *clientlist, fd_set *currfds)
{
	char *msg;
	//ssize_t recv(int sockfd, void *buf, size_t len, int flags);
	char read_buf[150001];
	memset(read_buf, 0, 150001);
	int read_bytes = recv(fd, read_buf, 150000, 0);
	//if client closed connection
	if (read_bytes == 0)
	{
		//prep leaving message
		msg = generate_msg(clientlist[fd].id, NULL, CLIENTLEFT);
		
		//delete client from client_arrays
		delete_client(fd, clientlist);

		// delete fd from fd set currfds
		FD_CLR(fd, currfds);
	}
	//if there's a message
	else if (read_bytes > 0)
		msg = generate_msg(clientlist[fd].id, read_buf, NEWMESSAGE);
	deliver_msg(msg, clientlist);
		
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
	
	// add server_socket to currfds;
	// void FD_SET(int fd, fd_set *set); -> add an fd to a specified fd set
	FD_SET(server_socket, &currfds);
	
	//set up server loop
	while (1)
	{
		// copy fds from currfds to readfds and writefds
		readfds = currfds;
		writefds = currfds;

		// int select(int numfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout); 
		// numfds should be set to highest numbered fd in any set plus 1
		// select checks all fds inside fdset and will adjust fdsets as fit (readfds have readable fds, writefds have writable fds)
		// if select errors out, close all fds and exit program
		if (select(maxfd + 1, &readfds, &writefds, NULL, NULL) == -1)
			exit_program(clientlist);
		
		// iterate over all fds in currfds
		// int FD_ISSET(int fd, fd_set *set); -> checks if fd is part of specified fd set
		for (int fd = 0; fd <= maxfd; fd++)
		{
			//if server_socket is readable, add new client
			if (FD_ISSET(server_socket, &readfds))
				add_new_client(server_socket, clientlist, &currfds);

			//if client fd can be read from, read message sent from client to server
			else if (FD_ISSET(fd, &readfds))
				read_msg(fd, clientlist, &currfds);
			
			// if fd can be written to, send all messages from client mailbox
			else if (FD_ISSET(fd, &writefds))
				send_msg(fd, clientlist);
		}
		printf("finished one round\n");
	}
	close(server_socket); //when do we do this?
	return (0);
	
}
