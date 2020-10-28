// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <time.h>
  
#define PORT     0710 
#define MAXLINE 1024
typedef struct message_struct { char data[128]; time_t client_out; time_t client_in; time_t server_out; time_t server_in; time_t time_travel;} message_struct; 
  
// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello World from server";
	struct message_struct *buffer_strct = (struct message_struct*) malloc(sizeof(struct message_struct));
	struct message_struct *message_send = (struct message_struct*) malloc(sizeof(struct message_struct));
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
  	printf("Listening...\n");

    while (1)
	{
		n = recvfrom(sockfd, (struct message_struct *)buffer_strct, sizeof(message_struct), MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
		
		buffer_strct->server_in = time(NULL);

		printf("\nPackage info:\n"); 
		printf("\nmessage: %s\n", buffer_strct->data);
		printf("\nclient_out: %lu\n", buffer_strct->client_out);
		printf("\nserver_in: %lu\n", buffer_strct->server_in);
		
		buffer_strct->server_out = time(NULL);
		sendto(sockfd, (struct message_struct *)buffer_strct, sizeof(message_struct), 0, (const struct sockaddr *) &cliaddr, len);  
		printf("Message sent.\n");
	}

	return 0;
}