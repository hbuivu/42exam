#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#define MAX_MSG_SIZE 1000000
typedef struct s_client
{
	int id;
	char *to_send;
}	t_client;

int next_client_id = 0;
int maxfd = 0;
int sockfd = 0;
fd_set readfds;
fd_set writefds;
fd_set currfds;
t_client clients[65536];
char send_buffer[MAX_MSG_SIZE];
char recv_buffer[MAX_MSG_SIZE];

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
	write(2, "Fatal Error\n", 13);
	exit(1);
}

void	send_to_all(int senderfd)
{
	for (int fd = 0; fd <= maxfd; fd++)
		if (FD_ISSET(fd, &writefds) && fd != senderfd && fd != sockfd)
			send(fd, send_buffer, strlen(send_buffer), 0);
}


int main(int argc, char **argv) 
{
	if (argc != 2)
	{
		write(2, "Wrong number of arguments\n", 27);
		exit(1);
	}

	//get port - does not need to be checked
	int port  = atoi(argv[1]);
	
	/***** CREATE LISTENING SOCKET *****/
	struct sockaddr_in servaddr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) 
		return_error();
	bzero(&servaddr, sizeof(servaddr)); 
	
	// assign IP, port
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port); 
  
	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) 
		return_error();

	if (listen(sockfd, 10) != 0) 
		return_error();
	
	//initiliaze fd sets
	FD_ZERO(&currfds);
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);

	FD_SET(sockfd, &currfds);

	maxfd = sockfd;

	while (1)
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
				{
					//get client fd
					struct sockaddr_in client_info;
					socklen_t len = sizeof(client_info);
					bzero(&client_info, sizeof(struct sockaddr_in));
					int clientfd = accept(sockfd, (struct sockaddr *)&client_info, &len);
					if (clientfd == -1)
						continue;
					
					//add client
					clients[clientfd].id = next_client_id;
					FD_SET(clientfd, &currfds);

					//set global variables
					next_client_id++;
					if (clientfd > maxfd)
						maxfd = clientfd;
					
					//send message
					sprintf(send_buffer, "server: client %i just arrived\n", clients[clientfd].id);
					send_to_all(clientfd);
				}
				else
				{
					//download message
					int bytes = recv(fd, recv_buffer, MAX_MSG_SIZE, 0);
					
					//client left
					if (bytes <= 0)
					{
						sprintf(send_buffer, "server: client %d just left\n", clients[fd].id);
						send_to_all(fd);
						free(clients[fd].to_send);
						clients[fd].to_send = NULL;
						FD_CLR(fd, &currfds);
						close(fd);
					}
					else
					{
						//add message to client mailbox
						//if we encounter a newline, send out a message
						//continue reading until all bytes are read
						recv_buffer[bytes] = '\0';
						clients[fd].to_send = str_join(clients[fd].to_send, recv_buffer);
						char *msg = NULL;
						while (extract_message(&(clients[fd].to_send), &msg))
						{
							sprintf(send_buffer, "client %d: %s", clients[fd].id, msg);
							send_to_all(fd);
							free(msg);
							msg = NULL;
						}
					}
				}

			}
		}
	}
	return (0);
}