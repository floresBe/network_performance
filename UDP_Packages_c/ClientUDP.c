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

	memset(&times, 0, sizeof(times)); 
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
	memset(&message_, 0, sizeof(message_));
	for (int i = 0; i < size_message - 8; i++)
	{ 
		strcat(message_, "x"); 
	}

	// printf("\nMessage to send: %s\n\n", message_);
	
	char message_json[MAXLINE];
	char time_[MAXLINE];

	// Sending messages number_packages times
	for (int i = 0; i < number_packages; i++)
	{
		memset(&message_json, 0, sizeof(message_json)); 
		strcpy(message_json, "{ \"data\" :\"");
		strcat(message_json, message_); 

		time_t client_out = time(NULL);
		strcat(message_json, "\", \"client_out\": "); 		
		sprintf(time_, "%ld", client_out);
		strcat(message_json, time_); 
		
		// printf("%s\n", message_json);
		
		sendto(sockfd, (char *)message_json, MAXLINE, 0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
	}

	clock_t start_time;
	clock_t end_t;
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

	float jitter ;
	float rate;

	memset(&jitter, 0, sizeof(jitter)); 
	memset(&rate, 0, sizeof(rate)); 
	
	jitter = get_average();
	rate = ((float) count_packages_received/(float)number_packages)* 100;

	printf("\n\nPackages sent: %d\n", number_packages);
	printf("Packages received: %d\n", count_packages_received);
	printf("Rate of packets received: %.0f \n", rate );
	printf("Jitter: %.2f\n", jitter);

	// file_name: server_hostname + size_message + jitter.txt

	char s_message[MAXLINE];
	char * file_data_jitter_name[MAXLINE];

	memset(&s_message, 0, sizeof(s_message)); 
	memset(&file_data_jitter_name, 0, sizeof(file_data_jitter_name)); 
	
	strcat(file_data_jitter_name, argv[1]);
	sprintf(s_message, "_%d", size_message);
	strcat(file_data_jitter_name, s_message);
	strcat(file_data_jitter_name, "_jitter");
	strcat(file_data_jitter_name, ".txt");

	print_to_file(file_data_jitter_name, number_packages, jitter);

	// file_name: server_hostname + size_message + jitter + gnu.txt
	char * file_data_jitter_gnu_name[MAXLINE];

	memset(&file_data_jitter_gnu_name, 0, sizeof(file_data_jitter_gnu_name)); 
	strcat(file_data_jitter_gnu_name, argv[1]);
	strcat(file_data_jitter_gnu_name, s_message);
	strcat(file_data_jitter_gnu_name, "_jitter");  
	strcat(file_data_jitter_gnu_name, "_gnu");
	strcat(file_data_jitter_gnu_name, ".txt"); 
	
	memset(&file_data_jitter_name, 0, sizeof(file_data_jitter_name)); 
	
	strcat(file_data_jitter_name, argv[1]);
	sprintf(s_message, "_%d", size_message);
	strcat(file_data_jitter_name, s_message);
	strcat(file_data_jitter_name, "_jitter");

	create_gnu_file(file_data_jitter_gnu_name, file_data_jitter_name);

	// file_name: server_hostname + size_message + rate.txt
 	
	char * file_data_rate_name[MAXLINE];
	memset(&file_data_rate_name, 0, sizeof(file_data_rate_name)); 

	strcat(file_data_rate_name, argv[1]);
	sprintf(s_message, "_%d", size_message);
	strcat(file_data_rate_name, s_message);
	strcat(file_data_rate_name, "_rate");  
	strcat(file_data_rate_name, ".txt"); 

	print_to_file(file_data_rate_name, number_packages, rate);

	char * file_data_rate_gnu_name[MAXLINE];

	memset(&file_data_rate_gnu_name, 0, sizeof(file_data_rate_gnu_name)); 
	strcat(file_data_rate_gnu_name, argv[1]);
	strcat(file_data_rate_gnu_name, s_message);
	strcat(file_data_rate_gnu_name, "_rate");  
	strcat(file_data_rate_gnu_name, "_gnu");
	strcat(file_data_rate_gnu_name, ".txt"); 
	
	memset(&file_data_rate_name, 0, sizeof(file_data_rate_name)); 
	
	strcat(file_data_rate_name, argv[1]);
	sprintf(s_message, "_%d", size_message);
	strcat(file_data_rate_name, s_message);
	strcat(file_data_rate_name, "_rate");

	create_gnu_file(file_data_rate_gnu_name, file_data_rate_name);

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

		char *message_json[MAXLINE];
		struct json_object *server_in;
		struct json_object *client_out;
		struct json_object *server_out;
		struct json_object *data;
		
		memset(&message_json, 0, sizeof(message_json)); 
		recvfrom(sockfd, (char *)message_json, MAXLINE ,MSG_WAITALL, ( struct sockaddr *) &servaddr, &len); 
		time_t client_in = time(NULL);
		
		i_package++;

		char current_time[MAXLINE];
    	struct tm *lt = localtime(&client_in);
    	strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", lt);

		printf("\n\nPackage %d received. Current time: %s (TZ=%s)\n", i_package, current_time, "PST8PDT");	
		// printf("%s\n", message_json);

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

		int time_travel;
		memset(&time_travel, 0, sizeof(time_travel)); 

		time_travel = (server_in_int - client_out_int) + (client_in_int - server_out_int);

		// printf("server_in_int %d\n", server_in_int);
		// printf("client_out_int %d\n", client_out_int);
		// printf("client_in_int %d\n", client_in_int);
		// printf("server_out_int %d\n", server_out_int);

		// printf("time_travel %d\n", time_travel);
		
		(times)[i_package] = time_travel;
		// printf("(times)[%d] = %d\n", i_package, (times)[i_package]);

		pthread_mutex_lock(&mutex_packages_received);
		
		//count every received package 
		count_packages_received ++;
		
		pthread_mutex_unlock(&mutex_packages_received);
	}
}

float get_average(){
	float avrg = 0;

	for (int i = 0; i < i_package - 1; i++)
	{  
		// printf("(times)[%d + 1] = %d,\n(times)[%d] = %d\n", i, (times)[i + 1], i, (times)[i]);
		int difference = (times)[i + 1] - (times)[i]; 
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

int print_to_file(char * filename, int data1, float data2)
{
   FILE * ou_file; 
   
   ou_file = fopen(filename, "a+");
   
   if ( !ou_file ) {
      return 0;
   }
	
	fprintf(ou_file,"%d\t%.2f\n", data1, data2);

   return 1;
}

int create_gnu_file(char * filename, char * file_data_name)
{
   FILE * ou_file; 
   
   ou_file = fopen(filename, "w");
   
   if ( !ou_file ) {
      return 0;
   }
	
	fprintf(ou_file,"#!/usr/local/bin/gnuplot -persist\n");
	fprintf(ou_file,"set terminal postscript landscape noenhanced color  \\ \n");
	fprintf(ou_file, "dashed defaultplex \" Helvetica \" 14\n");
	fprintf(ou_file,"set output '%s_grafica.ps'\n", file_data_name);
	fprintf(ou_file,"set xlabel \"Tamano del paquete UDP\" \n");
	fprintf(ou_file,"set ylabel \"Tasa de paquetes perdidos (%c) \" \n", '%');
	fprintf(ou_file,"set key left top\n");
	fprintf(ou_file,"set title \"Tasa de paquetes perdidos en funcion del tamano del buffer UDP\" \n");
	fprintf(ou_file,"#set xrange [ 0 : 130 ] noreverse nowriteback\n");
	fprintf(ou_file,"#set yrange [ -.5 : 14 ] noreverse nowriteback\n");
	fprintf(ou_file,"#set mxtics 5.000000\n");
	fprintf(ou_file,"#set mytics 1.000000\n");
	fprintf(ou_file,"#set xtics border mirror norotate 1\n");
	fprintf(ou_file,"#set ytics border mirror norotate 0.5\n");
	fprintf(ou_file,"plot \"%s.txt\" using 1:2 title \"Tasa de paquetes perdidos\" w lp pt 5 pi -4 lt rgb \"violet\", \\ \n", file_data_name);
	fprintf(ou_file,"#w linespoint\n");
	fprintf(ou_file,"#    EOF\n");


   return 1;
}

