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
	char *msg;
	int id;
}	t_client;

int maxfd = 0;
int sockfd = 0;
int next_client_id = 0;

fd_set writefds;
fd_set readfds;
fd_set currfds;

t_client clients[65536];
char sendbuf[MAX_MSG_SIZE];
char recvbuf[MAX_MSG_SIZE];

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
			send(fd, sendbuf, strlen(sendbuf), 0);
}

void	add_client()
{
	struct sockaddr_in cli;
	socklen_t len = sizeof(cli);
	bzero(&cli, sizeof(cli)); 
	int clientfd = accept(sockfd, (struct sockaddr *)&cli, &len);
	if (clientfd < 0)
		return ;
	
	//add client to array and set
	clients[clientfd].id = next_client_id;
	FD_SET(clientfd, &currfds);

	//update global variables
	next_client_id++;
	if (clientfd > maxfd)
		maxfd = clientfd;

	//send message
	sprintf(sendbuf, "server: client %d just arrived\n", clients[clientfd].id);
	send_to_all(clientfd);     
}

void	disconnect_client(int fd)
{
	sprintf(sendbuf, "server: client %d just left\n", clients[fd].id);
	send_to_all(fd);

	free(clients[fd].msg);
	clients[fd].msg = NULL;
	FD_CLR(fd, &currfds);
	close(fd);
}

void	send_message(int bytes, int fd)
{
	recvbuf[bytes] = '\0';
	clients[fd].msg = str_join(clients[fd].msg, recvbuf);
	char *msg = NULL;
	while (extract_message(&(clients[fd].msg), &msg))
	{
		sprintf(sendbuf, "client %d: %s", clients[fd].id, msg);
		send_to_all(fd);
		free(msg);
		msg = NULL;
	}
}


int main(int argc, char **argv) 
{
	if (argc != 2)
	{
		write(2, "Wrong number of arguments\n", 27);
		exit(1);
	}

	int port = atoi(argv[1]);

	//create listen socket
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
	
	//initialize sets
	FD_ZERO(&writefds);
	FD_ZERO(&readfds);
	FD_ZERO(&currfds);

	//add sockfd to currfds
	FD_SET(sockfd, &currfds);

	//set maxfd
	maxfd = sockfd;

	while(1)
	{
		readfds = currfds;
		writefds = currfds;

		if (select(maxfd + 1, &readfds, &writefds, NULL, NULL) == -1)
			continue;
		for (int fd = 0; fd <= maxfd; fd++)
		{
			if (FD_ISSET(fd, &readfds))
			{
				if (fd == sockfd)
					add_client();
				else
				{
					int bytes = recv(fd, recvbuf, MAX_MSG_SIZE, 0);
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
