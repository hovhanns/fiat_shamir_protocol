#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socket/socket.h"
#include "numbers/numbers.h"


void usage()
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "./client -i <ip> -p <port>\n");
	fprintf(stderr, "  -i: server IP\n");
	fprintf(stderr, "  -p: server port\n");
	exit(1);
}
/**
 * @return 	-1	fail
 *          >0	number  
 */
int get_n()
{
	FILE *fp;
	int ret = -1;
	fp = fopen("trust.txt", "r");
	if (fp==NULL)
	{
		printf("can't open file\n");
		return ret;
	}

	fscanf(fp, "%d", &ret);
	fclose(fp);

	return ret;
}

int start_swoping_with_server(int serv_id, int s, int n)
{
	char m_send[100];
	memset(m_send, 0,100);

	int v = (s*s) % n; /// must publish	
	printf("v = %d\n", v);
	sprintf(m_send, "%d", v);	
	if(send(serv_id, m_send, strlen(m_send), 0) < 0)
	{
		printf("can't send\n");
		return -1;
	}
	char *tmp = NULL;
	tmp = get_data_from_socket(serv_id);
	if(!strstr(tmp,"@"))
	{		
		return -1;
	}
	free(tmp);

	
	int r = GetRandomNumber(1, n);
	int x = (r*r) % n; //// must sent to server
	printf("x = <%d>\n", x);
	memset(m_send, 0,100);
	sprintf(m_send, "%d", x);	
	if(send(serv_id, m_send, strlen(m_send), 0) < 0)
	{
		printf("can't send\n");
		return -1;
	}

	char *ret = NULL;
	ret = get_data_from_socket(serv_id);

	int e = atoi(ret);
	printf("e = <%d>\n", e);
	int a = (r*((e==0)?1:s))%n;
	memset(m_send, 0,100);
	sprintf(m_send, "%d", a);	
	printf("a = %d\n", a);
	if(send(serv_id, m_send, strlen(m_send), 0) < 0)
	{
		printf("can't send\n");
		return -1;
	}
	free(ret);
	ret = NULL;


	ret = get_data_from_socket(serv_id);
	printf("finnally - <%s>\n",ret );


}
int main(int argc, char *argv[])
{
	if (argc < 4)
	{
		usage();		
	}

	int opt = -1;

	char port[10];
	memset(port, 0, 10);
	char ip[16];
	memset(ip, 0, 16);

	while((opt = getopt(argc, argv, "հi:p:")) > 0)
	{
		switch(opt)
		{
			case 'h':
				usage();
				break;
			case 'p':
				strcpy(port, optarg);
				break;
			case 'i':
				strcpy(ip, optarg);
				break;
			default:
				printf("Unknown option %c\n", opt);
				usage();				
		}
	}


	int n = get_n();
	if (n==-1)
	{
		printf("can't get public number\n");
		return 0;
	}
	printf("n = %d\n", n);

	int serv_id = socket_init(ip, port);

	int s = getCoprime(n);

	printf("s = %d\n", s);
	
	int i;
	for (i = 0; i < 5; ++i)
	{
		printf("round %d\n", i);
		start_swoping_with_server(serv_id, s, n);
		sleep(1);

	}
	return 0;
}

