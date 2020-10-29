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
typedef struct message_struct { char data[128]; time_t client_out; time_t client_in; time_t server_out; time_t server_in; time_t time_travel;} message_struct; 

static void time_convert(time_t t0, char const *tz_value);

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
		
		setenv("TZ", "PST8PDT", 1);
		buffer_strct->server_in = time(NULL);

		char buffer[64];
    	struct tm *lt = localtime(&buffer_strct->server_in);
    	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", lt);
		
		printf("\nPackage received:"); 
    	printf("\nCurrent time: %ld = %s (TZ=%s)\n", (long)buffer_strct->server_in, buffer, "PST8PDT");	
		// printf("\nmessage: %s\n", buffer_strct->data); 
		
		int n =0;	
		while(n < 10000 * 10){
			n++;
			printf( "%d\r",n);
		}

		buffer_strct->server_out = time(NULL);

		sendto(sockfd, (struct message_struct *)buffer_strct, sizeof(message_struct), 0, (const struct sockaddr *) &cliaddr, len);  
		printf("\nMessage sent.\n");
	}

	return 0;
}