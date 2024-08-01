#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_client
{
    char *msg;
    int id;
}   t_client;

fd_set writefds;
fd_set readfds;
fd_set currfds;

int maxfd = 0;
int sockfd = 0;
int client_id = 0;

t_client clients[4096];
char sendbuf[1000000];
char recvbuf[1000000];

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

void    return_error()
{
    write(2, "Fatal error\n", 13);
    exit(1);
}

void    send_to_all(int senderfd)
{
    for (int fd = 0; fd <= maxfd; fd++)
        if (FD_ISSET(fd, &writefds) && fd != senderfd && fd != sockfd)
            send(fd, sendbuf, strlen(sendbuf), 0);
}

void    add_client()
{
    struct sockaddr_in cli;
    bzero(&cli, sizeof(cli));
    socklen_t len = sizeof(cli);
	int clientfd = accept(sockfd, (struct sockaddr *)&cli, &len);
	if (clientfd < 0)
        return ;
    
    //add client to set and array
    clients[clientfd].id = client_id;
    FD_SET(clientfd, &currfds);

    //adjust global variables
    client_id++;
    if (maxfd < clientfd)
        maxfd = clientfd;

    //send message
    sprintf(sendbuf, "server: client %d just arrived\n", clients[clientfd].id);
    send_to_all(clientfd);
}

void    remove_client(int fd)
{
    //free, clear, close
    free(clients[fd].msg);
    clients[fd].msg = NULL;
    FD_CLR(fd, &currfds);
    FD_CLR(fd, &writefds);
    FD_CLR(fd, &readfds);
    close (fd);

    //send message
    sprintf(sendbuf, "server: client %d just left\n", clients[fd].id);
    send_to_all(fd);
}

void    send_message(int bytes, int fd)
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

    //create listening socket
	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1)
        return_error();
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	// servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //127.0.0.1
	servaddr.sin_port = htons(port); 
  
	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
        return_error();
	if (listen(sockfd, 10) != 0)
        return_error();
    
    //init fd_sets
    FD_ZERO(&writefds);
    FD_ZERO(&readfds);
    FD_ZERO(&currfds);

    //add sockfd to currfds
    FD_SET(sockfd, &currfds);

    //set maxfd
    maxfd = sockfd;

    while (1)
    {
        writefds = currfds;
        readfds = currfds;

        if (select(maxfd + 1, &readfds, &writefds, NULL, NULL) == -1)
            continue ;
        for (int fd = 0; fd <= maxfd; fd++)
        {
            if (FD_ISSET(fd, &readfds))
            {
                if (fd == sockfd)
                    add_client();
                else
                {
                    int bytes = recv(fd, recvbuf, 1000000, 0);
                    if (bytes <= 0)
                        remove_client(fd);
                    else
                        send_message(bytes, fd);
                }
            }
        }
    }
    return (0);
	
}
