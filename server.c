#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "socket/socket.h"
#include "numbers/numbers.h"

void usage()
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "./server -p <port>\n");
	fprintf(stderr, "  -h: prints this help text\n");
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


/**
 * 
 * @param  clinet_id [client's socket id]
 * @param  n         [public key, shared from trusted center]
 * @return           [-1	fail]
 *                   [1		OK]    	
 *                   [0		numbers don't mutch]
 */
int start_swoping_with_client(int clinet_id, int n)
{
	char m_send[100];
	char *gt = NULL;
	memset(m_send, 0,100);
		
	
	gt = get_data_from_socket(clinet_id);	
	int v = atoi(gt);	
	printf("v = %d\n", v);

	free(gt);
	gt = NULL;

	if(send(clinet_id, "@", strlen("@"), 0) < 0)
	{
		printf("can't send\n");
		return -1;
	}

	gt = get_data_from_socket(clinet_id);	
	int x = atoi(gt);
	printf("x = <%d>\n", x);

	free(gt);
	gt = NULL;
	
	int e = GetRandomNumber(0,2);
	printf("e = <%d>\n", e);
	sprintf(m_send, "%d", e);	
	if(send(clinet_id, m_send, strlen(m_send), 0) < 0)
	{
		printf("can't send\n");
		return -1;
	}

	gt = get_data_from_socket(clinet_id);

	int a = atoi(gt);
	printf("a = %d\n", a);
	if ((a*a)%n == ((e==0?1:v)*x) %n )
	{
		if(send(clinet_id, "Ok", strlen("Ok"), 0) < 0)
		{
			printf("can't send\n");
			return -1;
		}
		return 1;
	}
	else
	{
		if(send(clinet_id, "error", strlen("error"), 0) < 0)
		{
			printf("can't send\n");
			return -1;
		}
		return 0;
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		usage();		
	}
	int opt = -1;
	int n = -1;
	int i = -1;
	int sum = 0;
	char port[10];
	memset(port, 0, 10);


	while((opt = getopt(argc, argv, "հp:")) > 0)
	{
		switch(opt)
		{
			case 'h':
				usage();
				break;
			case 'p':
				strcpy(port, optarg);
				break;
			default:
				printf("Unknown option %c\n", opt);
				usage();				
		}
	}

	n = get_n();
	if (n==-1)
	{
		printf("can't get public number\n");
		return 0;
	}
	printf("n = %d\n", n);
	int clinet_id = Listener(port);

	
	for (i = 0; i < 5; ++i)
	{
		printf("round %d\n",i );
		sum+=start_swoping_with_client(clinet_id, n);
		sleep(1);
	}
	if (sum==5)
	{
		printf("peron is trusted!!\n");
		
	}
	return 0;
}



