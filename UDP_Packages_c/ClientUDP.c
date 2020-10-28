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
  
#define PORT     0710 
#define MAXLINE 1024

void * listen_server();
double get_average();
pthread_t thread_listen_server;
pthread_mutex_t mutex_packages_received;

typedef struct message_struct { char data[128]; time_t client_out; time_t client_in; time_t server_out; time_t server_in; time_t time_travel;} message_struct; 
struct message_struct *message_send;
char *message[128]; 
char message_[128];
int size_message, number_packages, count_packages_received = 0;

int sockfd; 
char buffer[MAXLINE]; 
struct message_struct *buffer_strct;

struct sockaddr_in servaddr; 
struct hostent *he;
int n, len;

int ** times;
int i_package = 0;

// Driver code 
int main(int argc, char *argv[]) {	
	// Verifing args
	if (argc != 4) {
		fprintf(stderr,"It should be used like this: host_name size_message number_packages\n");
		exit(1);
	}

	// Geting the server information
	if ((he=gethostbyname(argv[1])) == NULL) {  
		herror("Function gethostbyname failed");
		exit(1);
	}

	size_message = atoi(argv[2]);
	number_packages = atoi(argv[3]);

	if(size_message > 128){
		printf("size_message should be smaller than 128");
		exit(1);
	}

	if ( !create_array(&times, number_packages)) {
      fprintf(stderr,"Error: Unable to create array\n");
      return 0;
   }

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("Socket creation failed"); 
		exit(EXIT_FAILURE); 
	}

	// Start thread to listen for server
	if (pthread_mutex_init(&mutex_packages_received, NULL) != 0) {
		perror("Failed to start mutex initialization");
		exit(EXIT_FAILURE);
	}

	pthread_create(&thread_listen_server, NULL, listen_server, NULL);

	// Filling server information 
	memset(&servaddr, 0, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT);  
	servaddr.sin_addr = *((struct in_addr *)he->h_addr);

	// Creating message
	for (int i = 0; i < size_message - 8; i++)
	{ 
		strcat(message_, "x"); 
	}
	message_struct *message_send = (struct message_struct*) malloc(sizeof(struct message_struct));
	strcpy(message_send->data, message_);
	printf("\nMessage: %s\n\n", message_send->data);

	// Sending messages number_packages times
	for (int i = 0; i < number_packages; i++)
	{
		message_send->client_out = time(NULL);
		sendto(sockfd, (struct message_struct *)message_send, sizeof(message_struct), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
	}

	clock_t start_time ;
	clock_t end_t ; 
	double seconds_elapsed = 0;  
	start_time = clock();
	// printf("start_time: %lu\n",start_time);
	double timeout = number_packages * 0.1; //100 millisecond for every send message

	// Timeout wating
    while ( seconds_elapsed < timeout * 1000) {
		end_t = clock();
		// seconds_elapsed = (double)(end_t - start_time) / CLOCKS_PER_SEC;
		seconds_elapsed = (double)(end_t - start_time) / CLOCKS_PER_SEC * 1000;
		// printf( "[WAITING FOR %.0f Of %.0f SECONDS ]", seconds_elapsed, timeout);
		printf( "[WAITING FOR %.0f Of %.0f MILISECONDS ]", seconds_elapsed, timeout * 1000);
		printf( "\r");
	}
 	// printf("\nend_t: %lu\n",end_t);

	pthread_mutex_lock(&mutex_packages_received);

	double jitter = get_average();

	printf("\n\nPackages sent: %d\n", number_packages);
	printf("Packages received: %d\n", count_packages_received);
	printf("Rate of packets received: %d\n", (count_packages_received/number_packages)*100);
	printf("Jitter: %f\n", jitter);

	pthread_mutex_unlock(&mutex_packages_received);
	
	//Cancel thread
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
	
	//No more packages should be received than sent
	while (count_packages_received < number_packages){
		// n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
		message_struct *buffer_strct = (struct message_struct*) malloc(sizeof(struct message_struct));
		recvfrom(sockfd, (struct message_struct *)buffer_strct, sizeof(message_struct),MSG_WAITALL, ( struct sockaddr *) &servaddr, &len); 

		// printf("Server message: %s\n", buffer_strct->data);

		buffer_strct->client_in = time(NULL);

		printf("\nPackage info:\n"); 
		printf("\nmessage: %s\n", buffer_strct->data);
		printf("\nclient_out: %lu\n", buffer_strct->client_out);
		printf("\nserver_in: %lu\n", buffer_strct->server_in);
		printf("\nserver_out: %lu\n", buffer_strct->server_out);
		printf("\nclient_in: %lu\n", buffer_strct->client_in);

		buffer_strct->time_travel = (buffer_strct->server_in - buffer_strct->client_out) + (buffer_strct->client_in - buffer_strct->server_out);

		(times)[i_package] = (int) buffer_strct->time_travel;
		i_package++;

		printf("\ntime_travel: %d\n",(times)[i_package]);

		pthread_mutex_lock(&mutex_packages_received);
		
		//count every received package 
		count_packages_received ++;
		
		pthread_mutex_unlock(&mutex_packages_received);

		buffer[n] = '\0';
	}
}

double get_average(){
	double avrg = 0;

	for (int i = 0; i < i_package; i++)
	{
		double difference = (times)[i_package + 1] - (times)[i_package];
		printf("difference: %f\n", difference);
		avrg += difference;
	}
	
	avrg = avrg/i_package;
	printf("avrg: %f\n", avrg);
	return avrg;
}

int create_array(int **array, int c)
{
   (*array) = (int *) malloc(c * sizeof(int));
   if ( !(*array) ) {
      return 0;
   }
   return 1;
}

