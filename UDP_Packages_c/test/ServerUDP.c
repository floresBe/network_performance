// Server side implementation of UDP client-server model 
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <time.h>
  
#define PORT     6050 
#define MAXLINE 1024 

// Driver code 
int main() {

	int sockfd; 
	struct sockaddr_in servaddr, cliaddr;
	
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
	    perror("Socket creation failed"); 
	    exit(EXIT_FAILURE); 
	} 
	printf("Socket created.\n");
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family    = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
	{
		perror("Bind failed"); 
		exit(EXIT_FAILURE); 
	}
	printf("Binded\n");

	int len, n;
	len = sizeof(cliaddr);  //len is value/resuslt 
	
	setenv("TZ", "PST8PDT", 1); // set time zone
	printf("Set time zone\n");
	printf("Listening...\n");
	
	while (1)
	{
		char message[MAXLINE];
		memset(&message, 0, sizeof(message));   
		n = recvfrom(sockfd, (char *)message, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
		time_t server_in = time(NULL);

		char current_time[MAXLINE];
		struct tm *lt = localtime(&server_in);
		strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", lt);

		printf("\nPackage received: %s Current time: %s \n", message, current_time);	
		
		sendto(sockfd, (char *)message, MAXLINE, 0, (const struct sockaddr *) &cliaddr, len);
	}

	return 0;
}