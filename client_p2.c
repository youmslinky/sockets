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
	uint16_t serverport = 20183;
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
		fprintf(stderr, "usage: client_p2 hostAddress [port]\n");
		return 1;
	}
	/* translate host name into peer's IP address */
	hp = gethostbyname(host);
	if (!hp) {
		fprintf(stderr, "client_p2: unknown host: %s\n", host);
		return 1;
	}
	/* build address data structure */
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
	sin.sin_port = htons(serverport);
	/* active open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("client_p2: socket");
		return 1;
	}
	printf("Connecting...\n");
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("client_p2: connect");
		shutdown(s,-2);
		return 1;
	}
	printf("Successfully connected to server on port %u.\n",serverport);
	/* main loop: get and send lines of text */
	printf("client MSG: ");
	int sendresult = 0;
	while(1){
		while (fgets(buf, sizeof(buf), stdin)) {
			buf[MAX_LINE-1] = '\0';
			len = strlen(buf) + 1;
			sendresult = send(s, buf, len, MSG_CONFIRM);
			//printf("send return val:%d\n",sendresult);
			//printf("Client MSG: ");
			len = recv(s, buf, sizeof(buf), 0);
			printf("SVR_MSG_RCV'd: %s",buf);
			printf("client MSG: ");
		}
	}
	return 0;
}
