#include "lib/unp.h"

int main( int argc, char* argv[] ) {
	setvbuf(stdout, NULL, _IONBF, 0);
	int 		n, connfd, sockfd, port;
	int 		stdineof = 0;
	socklen_t 	clilen;
	char 		buf[MAXLINE];
	struct 		sockaddr_in 	cliaddr, servaddr;

	if (argc != 2) { fprintf(stderr, "ERROR: Invalid Command Line format\n"); return EXIT_FAILURE; }
	port = atoi(argv[1]) + 9877;

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));			// Zero out memory
	servaddr.sin_family 		= AF_INET;
	servaddr.sin_addr.s_addr 	= htonl(INADDR_ANY);
	servaddr.sin_port 		= htons(port);	

	Bind(sockfd, (SA *) &servaddr, sizeof(servaddr));	// Bind to local address
 	Listen(sockfd, LISTENQ);				// Listen for connections
	
	for ( ; ; ) {	
		clilen = sizeof(cliaddr);
		connfd = Accept(sockfd, (SA *) &cliaddr, &clilen);
		if ( (n = Read(fileno(stdin), buf, MAXLINE)) == 0) {
			Shutdown(sockfd, SHUT_WR);
			return EXIT_SUCCESS; 
		}
		Writen(connfd, buf, n);
		Close(connfd);
	}
	return EXIT_SUCCESS;
}
