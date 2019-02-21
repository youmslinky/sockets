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
	uint16_t serverport = 5432;
	char buf[MAX_LINE];
	int s;
	int len;
	if (argc==3) {
		host = argv[1];
		serverport = strtoul(argv[2],NULL,0);
	}
	else if (argc==2) {
		host = argv[1];
	}
	else {
		fprintf(stderr, "usage: program host port\n");
		return 1;
	}
	/* translate host name into peer's IP address */
	hp = gethostbyname(host);
	if (!hp) {
		fprintf(stderr, "client_p1: unknown host: %s\n", host);
		return 1;
	}
	/* build address data structure */
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
	sin.sin_port = htons(serverport);
	/* active open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket binding error");
		return 1;
	}
	printf("Connecting...\n");
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("simplex-talk: connect");
		shutdown(s,-2);
		return 1;
	}
	printf("Successfully connected to server on port %u\n\n",serverport);
	/* main loop: get and send lines of text */
	printf("client MSG: ");
	while (fgets(buf, sizeof(buf), stdin)) {
		buf[MAX_LINE-1] = '\0';
		len = strlen(buf) + 1;
		send(s, buf, len, MSG_CONFIRM);
		printf("client MSG: ");
	}
	return 0;
}
