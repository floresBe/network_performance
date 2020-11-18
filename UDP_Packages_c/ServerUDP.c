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
#include<json-c/json.h>
  
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
		char message_json[MAXLINE];
		
		n = recvfrom(sockfd, (char *)message_json, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
		time_t server_in = time(NULL);
		message_json[n] = '\0';  

		char buffer_time[64];
    	struct tm *local_time = localtime(&server_in);
    	strftime(buffer_time, sizeof(buffer_time), "%Y-%m-%d %H:%M:%S", local_time);
		
		printf("\nPackage received. Current time: %s (TZ=%s)\n", buffer_time, "PST8PDT"); 

		// Simulated time traffic
		int n_ =0;	
		while(n_ < 10000 * 10){
			n_++;
			printf( "Simulated time traffic %d\r",n_);
		}

		time_t server_out = time(NULL);

		char time_[128];
		strcat(message_json, ", \"server_in\": "); 
		sprintf(time_, "%ld", server_in);
		strcat(message_json, time_); 
		
		strcat(message_json, ", \"server_out\": "); 
		sprintf(time_, "%ld", server_out);
		strcat(message_json, time_); 
		
		strcat(message_json, "} "); 
		
		sendto(sockfd, (char *)message_json, MAXLINE, 0, (const struct sockaddr *) &cliaddr, len);
	}

	return 0;
}