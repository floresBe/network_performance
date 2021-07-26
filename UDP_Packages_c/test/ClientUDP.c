// Client side implementation of UDP client-server model 

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <netdb.h>
#include <time.h>
#include<json-c/json.h>
  
#define PORT     6050 
#define MAXLINE 1024

void * listen_server();
float get_average();
int print_to_file(char * filename, int data1, float data2);

pthread_t thread_listen_server;
pthread_mutex_t mutex_packages_received;

char message_[128];
int size_message, count_packages_received = 0;

int sockfd; 
struct sockaddr_in servaddr; 
struct hostent *he;

int len;

int i_package = 0;

// Driver code 
int main(int argc, char *argv[]) {	
	// Verifing args
	if (argc != 2) {
		fprintf(stderr,"It should be used like this: host_name\n");
		exit(1);
	}

	// Geting the server information
	if ((he=gethostbyname(argv[1])) == NULL) {  
		herror("Function gethostbyname failed");
		exit(1);
	} 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("Socket creation failed"); 
		exit(EXIT_FAILURE); 
	}

	pthread_create(&thread_listen_server, NULL, listen_server, NULL);

	// Filling server information 
	memset(&servaddr, 0, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT);  
	servaddr.sin_addr = *((struct in_addr *)he->h_addr);

	// Creating message
	memset(&message_, 0, sizeof(message_));
    strcat(message_, "Hola Mundo!"); 

	// Sending messages
	while (1)
	{				
		sendto(sockfd, (char *)message_, MAXLINE, 0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
	}
		
	// Cancel thread
    void *res;
	int s = pthread_cancel(thread_listen_server);
    if (s != 0)
        perror("pthread_cancel");

    s = pthread_join(thread_listen_server, &res);
    if (s != 0)
        perror("pthread_join");

	close(sockfd); 
	return 0; 
}

void * listen_server(){
	
	while (1){

		char *message_json[MAXLINE];
		
		memset(&message_json, 0, sizeof(message_json)); 
		recvfrom(sockfd, (char *)message_json, MAXLINE ,MSG_WAITALL, ( struct sockaddr *) &servaddr, &len); 
		time_t time_in = time(NULL);
		
		char current_time[MAXLINE];
    	struct tm *lt = localtime(&time_in);
    	strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", lt);
		
		i_package++;
		printf("\n\nPackage %d received: %s Current time: %s\n", i_package, message_json, current_time);	
	}
}
 