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
	setenv("TZ", "PST8PDT", 1); //set TZ
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
	// strcpy(message_send->data, message_);
	printf("\nMessage to send: %s\n\n", message_);
	
	char message_json[MAXLINE];
	char time_[MAXLINE];
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
		printf("message_json_send: %s\n", message_json);
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
		// printf( "[WAITING FOR %.0f Of %.0f MILISECONDS ]", seconds_elapsed, timeout * 1000);
		// printf( "\r");
	} 

	pthread_mutex_lock(&mutex_packages_received);

	double jitter = get_average();

	printf("\n\nPackages sent: %d\n", number_packages);
	printf("Packages received: %d\n", count_packages_received);
	printf("Rate of packets received: %d \n", (count_packages_received/number_packages)*100);
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
		message_struct *buffer_strct = (struct message_struct*) malloc(sizeof(struct message_struct));
		// recvfrom(sockfd, (struct message_struct *)buffer_strct, sizeof(message_struct),MSG_WAITALL, ( struct sockaddr *) &servaddr, &len); 
		
		char *message_json[1024];
		struct json_object *server_in;
		struct json_object *client_out;
		struct json_object *server_out;
		struct json_object *data;

		//  strcpy(message_json, "{ \"data\": \"x\", \"client_out\": 55, \"server_in\" : 55, \"server_out\" : 55 } "); 
		recvfrom(sockfd, (char *)message_json, MAXLINE ,MSG_WAITALL, ( struct sockaddr *) &servaddr, &len); 
		time_t client_in = time(NULL);
		
		char current_time[64];
    	struct tm *lt = localtime(&client_in);
    	strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", lt);

		printf("\nPackage received:\n"); 
		printf("\nCurrent time: %ld = %s (TZ=%s)\n", (long)client_in, current_time, "PST8PDT");	
		printf("\nmessage: %s\n", message_json);

		struct json_object *parsed_json;
		parsed_json = json_tokener_parse(message_json);
		json_object_object_get_ex(parsed_json, "data", &data);
		printf("data: %s\n", json_object_get_string(data));

		json_object_object_get_ex(parsed_json, "client_out", &client_out);
		printf("client_out: %d\n", json_object_get_int(client_out));
		
		json_object_object_get_ex(parsed_json, "server_in", &server_in);
		printf("server_in: %d\n", json_object_get_int(server_in));
		
		json_object_object_get_ex(parsed_json, "server_out", &server_out);
		printf("client_out: %d\n", json_object_get_int(server_out));

		printf("client_in: %ld\n\n", client_in);

		int client_out_int, server_in_int, server_out_int, client_in_int;
		
		server_in_int = json_object_get_int(server_in);
		printf("server_in: %d\n", server_in_int);

		client_out_int = json_object_get_int(client_out);
		printf("client_out: %d\n", client_out_int);

		 client_in_int = (long int) client_in;
		printf("client_in: %d\n", client_in);

		server_out_int = json_object_get_int(server_out);
		printf("server_out: %d\n", server_out_int);

		int time_travel = (server_in_int - client_out_int) + (client_in_int - server_out_int);
		// buffer_strct->time_travel = (json_object_get_int(server_in) - json_object_get_int(client_out)) + (buffer_strct->client_in - json_object_get_int(server_out));

		//buffer_strct->time_travel = (buffer_strct->server_in - buffer_strct->client_out) + (buffer_strct->client_in - buffer_strct->server_out);

		// (times)[i_package] = (int) buffer_strct->time_travel; 
		(times)[i_package] = time_travel; 
		printf("\ntime_travel[%d]: %d\n", i_package, (times)[i_package]);

		i_package++;

		pthread_mutex_lock(&mutex_packages_received);
		
		//count every received package 
		count_packages_received ++;
		
		pthread_mutex_unlock(&mutex_packages_received);

		buffer[n] = '\0';
	}
}

double get_average(){
	int avrg = 0; 
	
	printf("\n");

	for (int i = 0; i < count_packages_received - 1; i++)
	{
		printf("(times)[%d + 1] = %d,\n(times)[%d] = %d\n", i, (times)[i + 1], i, (times)[i]);
		int difference = (int) (times)[i + 1] - (int) (times)[i];
		printf("difference: %d\n", difference);
		avrg += difference;
	}
	avrg = avrg/count_packages_received;
	// printf("avrg: %d\n", avrg);
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

