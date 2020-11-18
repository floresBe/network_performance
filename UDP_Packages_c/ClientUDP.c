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

pthread_t thread_listen_server;
pthread_mutex_t mutex_packages_received;

typedef struct message_struct { char data[128]; time_t client_out; time_t client_in; time_t server_out; time_t server_in; time_t time_travel;} message_struct;

char message_[128];
int size_message, number_packages, count_packages_received = 0;

int sockfd; 
struct sockaddr_in servaddr; 
struct hostent *he;

int len;

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
	setenv("TZ", "PST8PDT", 1); //set Time Zone
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

	// printf("\nMessage to send: %s\n\n", message_);
	
	char message_json[MAXLINE];
	char time_[MAXLINE];
	memset(&message_json, 0, sizeof(message_json)); 
	
	// Sending messages number_packages times
	for (int i = 0; i < number_packages; i++)
	{
		strcpy(message_json, "{ \"data\" :\"");
		strcat(message_json, message_); 

		time_t client_out = time(NULL);
		strcat(message_json, "\", \"client_out\": "); 		
		sprintf(time_, "%ld", client_out);
		strcat(message_json, time_); 

		sendto(sockfd, (char *)message_json, MAXLINE, 0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
	}

	clock_t start_time;
	clock_t end_t ; 
	double seconds_elapsed = 0;  
	start_time = clock(); 
	double timeout = number_packages * 0.1; //100 millisecond for every send message

	// Timeout wating
    while ( seconds_elapsed < timeout * 1000) {
		end_t = clock(); 
		seconds_elapsed = (double)(end_t - start_time) / CLOCKS_PER_SEC * 1000; 
		printf( "[WAITING FOR %.0f Of %.0f MILISECONDS ]", seconds_elapsed, timeout * 1000);
		printf( "\r");
	} 

	pthread_mutex_lock(&mutex_packages_received);

	float jitter = get_average();

	printf("\n\nPackages sent: %d\n", number_packages);
	printf("Packages received: %d\n", count_packages_received);
	printf("Rate of packets received: %.0f \n",  ((float) count_packages_received/(float)number_packages)* 100);
	printf("Jitter: %.2f\n", jitter);

	pthread_mutex_unlock(&mutex_packages_received);
	
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
	
	//No more packages should be received than sent
	while (count_packages_received < number_packages){

		char *message_json[1024];
		struct json_object *server_in;
		struct json_object *client_out;
		struct json_object *server_out;
		struct json_object *data;

		recvfrom(sockfd, (char *)message_json, MAXLINE ,MSG_WAITALL, ( struct sockaddr *) &servaddr, &len); 
		time_t client_in = time(NULL);
		
		i_package++;

		char current_time[MAXLINE];
    	struct tm *lt = localtime(&client_in);
    	strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", lt);

		printf("\nPackage %d received. Current time: %s (TZ=%s)\n", i_package, current_time, "PST8PDT");	

		struct json_object *parsed_json;
		parsed_json = json_tokener_parse(message_json);
		json_object_object_get_ex(parsed_json, "data", &data); 

		json_object_object_get_ex(parsed_json, "client_out", &client_out);
		
		json_object_object_get_ex(parsed_json, "server_in", &server_in);
		
		json_object_object_get_ex(parsed_json, "server_out", &server_out);

		int client_out_int, server_in_int, server_out_int, client_in_int;
		
		server_in_int = json_object_get_int(server_in);

		client_out_int = json_object_get_int(client_out);

		client_in_int = (long int) client_in;

		server_out_int = json_object_get_int(server_out);

		int time_travel = (server_in_int - client_out_int) + (client_in_int - server_out_int);
		
		(times)[i_package] = time_travel; 

		pthread_mutex_lock(&mutex_packages_received);
		
		//count every received package 
		count_packages_received ++;
		
		pthread_mutex_unlock(&mutex_packages_received);
	}
}

float get_average(){
	float avrg = 0;

	for (int i = 0; i < count_packages_received - 1; i++)
	{  
		// printf("(times)[%d + 1] = %d,\n(times)[%d] = %d\n", i, (times)[i + 1], i, (times)[i]);
		int difference = (int) (times)[i + 1] - (int) (times)[i]; 
		// printf("difference: %d\n", difference);
		avrg += (float) difference;
	}
	// printf("sum: %f\n", avrg);
	avrg = avrg/(float)count_packages_received; 
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

