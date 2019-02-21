#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define SERVER_PORT 5432
#define MAX_PENDING 0
#define MAX_LINE 256
int main(int argc, char * argv[])
{
	struct sockaddr_in sin;
	uint16_t serverport = 20183;
	char buf[MAX_LINE];
	int len;
	int s, new_s;
	if (argc==2) {
		serverport = strtoul(argv[1],NULL,0);
	}
	else if(argc==1) {}
	else {
		fprintf(stderr, "usage: server_p1 [port]\n");
		return 1;
	}
	/* build address data structure */
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(serverport);
	/* setup passive open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("server_p1 error: socket");
		exit(1);
	}
	if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
		perror("server_p1 error: bind");
		exit(1);
	}
	listen(s, MAX_PENDING);
	/* wait for connection, then receive and print text */
	int connectionNumber = 1;
	printf("Server Started...\nServer Waiting for connections on port %u\n",serverport);
	while(1) {
		if ((new_s = accept(s, (struct sockaddr *)&sin, &len)) < 0) {
			perror("server_p1: accept error");
			exit(1);
		}
		printf("client #%d connected\n",connectionNumber);
		while ( (len = recv(new_s, buf, sizeof(buf), 0)) )
			printf("client #%d: RCVD: %s",connectionNumber,buf);
			//fputs(buf, stdout);
		printf("client #%d closed connection\n\n",connectionNumber);
		connectionNumber++;
		shutdown(new_s,2);
	}
}
