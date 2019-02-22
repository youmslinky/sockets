#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define SERVER_PORT 5432
#define MAX_LINE 256
int main(int argc, char * argv[])
{
	//FILE *fp;
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host;
	uint16_t serverport = 20183; //default port
	char buf[MAX_LINE];
	int s;
	int len;
	if (argc==3) {
		//port is provided
		host = argv[1]; //get host name from command line
		serverport = strtoul(argv[2],NULL,0); //get port from command line
	}
	else if (argc==2) {
		//port isn't provided
		host = argv[1]; //get host name from command line
	}
	else {
		//host was not specified, print error
		fprintf(stderr, "usage: client_p1 hostAddress [port]\n");
		return 1;
	}
	/* translate host name into peer's IP address */
	hp = gethostbyname(host);
	if (!hp) {
		fprintf(stderr, "client_p1: unknown host: %s\n", host);
		return 1;
	}
	/* build address data structure */
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
	sin.sin_port = htons(serverport);
	/* active open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("client_p1: socket error");
		return 1;
	}
	printf("Connecting...\n");
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		//error when connecting
		perror("client_p1: connect error");
		shutdown(s,-2);
		return 1;
	}
	printf("Successfully connected to server on port %u\n\n",serverport);
	/* main loop: get and send lines of text */
	printf("client MSG: "); //prompt user for input
	while (fgets(buf, sizeof(buf), stdin)) {
		buf[MAX_LINE-1] = '\0'; //make sure string sent is null terminated
		len = strlen(buf) + 1;
		send(s, buf, len, MSG_CONFIRM); //send message to server
		printf("client MSG: "); //prompt user for input again
	}
	return 0;
}
