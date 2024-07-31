#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_MSG_SIZE 1000000

typedef struct s_client
{
	char *to_send;
	int id;
}	t_client;

fd_set writefds;
fd_set readfds;
fd_set currfds;

int maxfd = 0;
int next_client_id = 0;
int sockfd = 0;

char send_buf[MAX_MSG_SIZE];
char recv_buf[MAX_MSG_SIZE];

t_client clients[65536];

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

void	return_error()
{
	write(2, "Fatal error\n", 13);
	exit(1);
}

void	send_to_all(int senderfd)
{
	for (int fd = 0; fd <= maxfd; fd++)
		if (FD_ISSET(fd, &writefds) && fd != senderfd && fd != sockfd)
			send(fd, send_buf, strlen(send_buf), 0);
}

void	add_client()
{
	//create new client fd
	struct sockaddr_in client_address;
	socklen_t len = sizeof(client_address);
	bzero(&client_address, sizeof(struct sockaddr_in));

	int new_client_fd = accept(sockfd, (struct sockaddr *)&client_address, &len);
	if (new_client_fd == -1)
		return ;
	
	//add client
	clients[new_client_fd].id = next_client_id;
	FD_SET(new_client_fd, &currfds);

	//set global variables
	if (maxfd < new_client_fd)
		maxfd = new_client_fd;
	next_client_id++;

	//send message
	sprintf(send_buf, "server: client %d just arrived\n", clients[new_client_fd].id);
	send_to_all(new_client_fd);
}

void	disconnect_client(int fd)
{
	//send message
	sprintf(send_buf, "server: client %d just left\n", clients[fd].id);
	send_to_all(fd);
	
	//remove from client array
	free(clients[fd].to_send);
	clients[fd].to_send = NULL;

	//remove from set
	FD_CLR(fd, &currfds);

	//close fd
	close(fd);
}

void	send_message(int bytes, int fd)
{
	recv_buf[bytes] = '\0';
	clients[fd].to_send = str_join(clients[fd].to_send, recv_buf);
	char *msg = NULL;
	while (extract_message(&(clients[fd].to_send), &msg))
	{
		sprintf(send_buf, "client %d: %s", clients[fd].id, msg);
		send_to_all(fd);
		free(msg);
		msg = NULL;
	}
}

int main(int argc, char **argv) 
{
	if (argc != 2)
	{
		write(2, "Wrong number of arguments\n", 26);
		exit(1);
	}

	//configure port
	int port = atoi(argv[1]);
	
	// socket create and verification
	struct sockaddr_in servaddr;  
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1)
		return_error(); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(port); 
  
	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		return_error();
	if (listen(sockfd, 10) != 0)
		return_error();
	
	//initialize fd sets
	FD_ZERO(&currfds);
	FD_ZERO(&writefds);
	FD_ZERO(&readfds);
	
	//add sockfd to the currfds to monitor
	FD_SET(sockfd, &currfds);

	//initialize maxfd to sockfd
	maxfd = sockfd;

	while (1)
	{
		//set write and read fd sets to currfds
		writefds = currfds;
		readfds = currfds;

		//monitor
		if (select(maxfd + 1, &readfds, &writefds, NULL, NULL) == -1)
			continue;
		
		//iterate over all fds up until maxfd
		for (int fd = 0; fd <= maxfd; fd++)
		{
			if (FD_ISSET(fd, &readfds))
			{
				if (fd == sockfd)
					add_client();
				else
				{
					//read message
					int bytes = recv(fd, recv_buf, MAX_MSG_SIZE, 0);
					if (bytes <= 0)
						disconnect_client(fd);
					else
						send_message(bytes, fd);
					
				}
			}
		}
	}
	return (0);
}
