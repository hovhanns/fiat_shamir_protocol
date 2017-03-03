#include "socket.h"

int socket_init(char *hostname, char *port)
{
	int sockid;
	struct sockaddr_in server;
	struct hostent        *he;
	sockid = socket(AF_INET, SOCK_STREAM, 0);
	if (sockid == -1)
	{
		Log("Could not create socket\n");
		return -1;
	}
	Log("hostname is: <%s>\n", hostname);
	if ( (he = gethostbyname(hostname)) == NULL )
	{
		Log("can't get host by name\n");
		return -1;
	}
	//printf("Socket created, ip:%s\n", ip);
	memcpy(&server.sin_addr, he->h_addr_list[0], he->h_length);
	//server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(port));
	if (connect(sockid , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		Log("connect failed. Error\n");
		return -1;
	}

	if(sockid == -1)
	{
		Log("error @ start connection\n" );
		return -1;
	}

	struct timeval timeout;
	timeout.tv_sec = 3; 						
	timeout.tv_usec = 0;
	if (setsockopt (sockid, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
	{
		Log("setsockopt RCVTIMEO failed\n");
	}


	if (setsockopt (sockid, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
	{
		Log("setsockopt RCVTIMEO failed\n");
	}

	Log("Connected to %s:%s\n",hostname,port );
	return sockid;
}


char *get_data_from_socket(int sock)
{
	char  server_reply[MSG_SIZE];
	memset(server_reply, '\0', MSG_SIZE);
	int bytes = 0;
	char * ret=NULL;
	ret = (char *) malloc(1* sizeof(char));
	ret[0] = '\0';
	while(1)
	{
		bytes = recv(sock,server_reply,MSG_SIZE,0);

		if(bytes==MSG_SIZE)
		{
			ret = (char*)realloc (ret, MSG_SIZE+strlen(ret)+1);
			strcat(ret, server_reply);
			memset(server_reply, '\0', MSG_SIZE);
		}
		else if(bytes> 0 && bytes< MSG_SIZE)
		{
			ret = (char*)realloc (ret, strlen(server_reply)+strlen(ret)+1);
			strcat(ret, server_reply);
			memset(server_reply, '\0', MSG_SIZE);			
			return ret;
		}
		else if(server_reply[0]=='\0' && bytes == -1)
		{			
			memset(server_reply, '\0', MSG_SIZE);
			return ret;
		}
		else
			return ret;
	}

	return ret;
}


int Listener(char *port)
{
	int option = 1;
	int sockid = -1,
		clientid = -1,
		c = -1,
		read_size = 1;

	struct sockaddr_in server, client;

	sockid = socket(AF_INET, SOCK_STREAM, 0);

	if (sockid == -1)
	{
		Log("Could nor create socket\n");
		return -1;
	}

	Log("Socket created\n");

	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(port));
	/* can use some port after closing program*/
	if (setsockopt(sockid, SOL_SOCKET, (SO_REUSEADDR), (char*)&option, sizeof(option)) < 0)
	{
		Log("setsockopt resuseaddr failed\n");
		close(sockid);
		return -1;
	}
	if (bind(sockid, (struct sockaddr *) &server, sizeof(server) ) < 0)
	{
		Log("bind failed\n");
		close(sockid);
		return -1;
	}
	Log("bind done\n");
	listen(sockid, 1);
	c = sizeof(struct sockaddr_in);	
	Log("waiting for connections...\n");
	clientid = accept(sockid, (struct sockaddr*) &client, (socklen_t*) &c);
	if (clientid < 0)
	{
		Log("accept failed\n");
		close(sockid);
		return -1;
	}

	Log("connection accepted\n");


	struct timeval timeout;
	timeout.tv_sec = 3; 						
	timeout.tv_usec = 0;
	if (setsockopt (clientid, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
	{
		Log("setsockopt RCVTIMEO failed\n");
		close(clientid);
		close(sockid);
		return -1;
	}


	if (setsockopt (clientid, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
	{
		Log("setsockopt RCVTIMEO failed\n");
		close(clientid);
		close(sockid);
		return -1;
	}

	return clientid;
}

