// Client side implementation of ZigBee client-server model 

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <json-c/json.h>
#include <xbee.h>

#define BUFFER_SIZE 256
#define SEND_TIME 10
#define MAXLINE 1024

// Functions
struct xbee * configure_xbee(struct xbee *xbee, xbee_err ret);
struct xbee_con * connection_xbee(struct xbee *xbee, struct xbee_con *con, xbee_err ret);
void receive_data(struct xbee *xbee, struct xbee_con *con, xbee_err ret);
void send_data(struct xbee *xbee, struct xbee_con *con, xbee_err ret);
void callback_function(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data);
void * listen_server(void *arg);
float get_average();
int print_to_file(char * filename, int data1, float data2);

pthread_t thread_listen_server;
pthread_mutex_t mutex_packages_received;

char message_[128];
int size_message, number_packages, count_packages_received = 0;
char read_buffer[BUFFER_SIZE];

int len;
int ** times;
int i_package = 0; 

//For thread_listen_server args
struct Server_ZigBee
{
	struct xbee *xbee;
	struct xbee_con *con;
	xbee_err ret;
};

// Driver code 
int main(int argc, char *argv[]) {	
	
	// Xbee errors variable
	xbee_err ret; 

	// Verifing args
	if (argc != 3) {
		fprintf(stderr,"It should be used like this: size_message number_packages\n");
		exit(1);
	}
	
	//To Do:
	// address = argv[1];
	// USBX = argv[x]

	size_message = atoi(argv[1]);
	number_packages = atoi(argv[2]);

	if(size_message > 128){
		printf("size_message should be smaller than 128");
		exit(1);
	}

	memset(&times, 0, sizeof(times)); 
	if ( !create_array(&times, number_packages)) {
      fprintf(stderr,"Error: Unable to create array\n");
      return 0;
   	}

	//set Time Zone
	setenv("TZ", "PST8PDT", 1); 

    // Configure xbee
	struct xbee *xbee = configure_xbee(xbee, ret);
	
	// Creating a new connection
	struct xbee_con *con = connection_xbee(xbee, con, ret);

	// Start thread to listen for server
	if (pthread_mutex_init(&mutex_packages_received, NULL) != 0) {
		perror("Failed to start mutex initialization");
		exit(EXIT_FAILURE);
	}

	//Server args for listen_server
	struct Server_ZigBee server_zigBee;
	server_zigBee.con = con;
	server_zigBee.ret = ret;
	server_zigBee.xbee = xbee;

	pthread_create(&thread_listen_server, NULL, listen_server, (void *)&server_zigBee);

	// Sending messages number_packages times
	for (int i = 0; i < number_packages; i++)
	{
		printf("package: %d - ",1+i);
		// Send data to the remote xbee 
		send_data(xbee, con, ret); 
	}
	
	clock_t start_time;
	clock_t end_t;
	double seconds_elapsed = 0; 
	double timeout = number_packages * 0.1; //100 millisecond for every send message
	start_time = clock();  

	// Timeout wating
    while ( seconds_elapsed < timeout * 1000) {
		// printf("%d\n", start_time);
		end_t = clock(); 
		seconds_elapsed = (double)(end_t - start_time) / CLOCKS_PER_SEC * 1000; 
		
		printf( "[WAITING FOR %.0f Of %.0f MILISECONDS ]", seconds_elapsed, timeout * 1000);
		printf( "\r");
	} 

	printf(" \n");
	 
	// Shutdown connection
	xbee_conEnd(con);
	// Shutdown libxbee
	xbee_shutdown(xbee); 

	// float jitter ;
	// float rate;

	// memset(&jitter, 0, sizeof(jitter)); 
	// memset(&rate, 0, sizeof(rate)); 
	
	// jitter = get_average();
	// rate = abs((((float) count_packages_received/(float)number_packages)* 100) - 100);

	// printf("\n\nPackages sent: %d\n", number_packages);
	// printf("Packages received: %d\n", count_packages_received);
	// printf("Rate of lost packets: %.0f \n", rate );
	// printf("Jitter: %.2f\n", jitter);
	
/*
	// file_name: server_hostname + number_packages + jitter.txt

	char s_message[MAXLINE];
	char * file_data_jitter_name[MAXLINE];

	memset(&s_message, 0, sizeof(s_message)); 
	memset(&file_data_jitter_name, 0, sizeof(file_data_jitter_name)); 
	
	strcat(file_data_jitter_name, argv[1]);
	sprintf(s_message, "_%d", number_packages);
	strcat(file_data_jitter_name, s_message);
	strcat(file_data_jitter_name, "_jitter");
	strcat(file_data_jitter_name, ".txt");

	print_to_file(file_data_jitter_name, size_message, jitter);

	// file_name: server_hostname + number_packages + jitter + gnu.txt
	char * file_data_jitter_gnu_name[MAXLINE];

	memset(&file_data_jitter_gnu_name, 0, sizeof(file_data_jitter_gnu_name)); 
	strcat(file_data_jitter_gnu_name, argv[1]);
	strcat(file_data_jitter_gnu_name, s_message);
	strcat(file_data_jitter_gnu_name, "_jitter");  
	strcat(file_data_jitter_gnu_name, "_gnu");
	strcat(file_data_jitter_gnu_name, ".txt"); 
	
	memset(&file_data_jitter_name, 0, sizeof(file_data_jitter_name)); 
	
	strcat(file_data_jitter_name, argv[1]);
	sprintf(s_message, "_%d", number_packages);
	strcat(file_data_jitter_name, s_message);
	strcat(file_data_jitter_name, "_jitter");

	create_gnu_file(file_data_jitter_gnu_name, file_data_jitter_name);

	// file_name: server_hostname + number_packages + rate.txt
 	
	char * file_data_rate_name[MAXLINE];
	memset(&file_data_rate_name, 0, sizeof(file_data_rate_name)); 

	strcat(file_data_rate_name, argv[1]);
	sprintf(s_message, "_%d", number_packages);
	strcat(file_data_rate_name, s_message);
	strcat(file_data_rate_name, "_rate");  
	strcat(file_data_rate_name, ".txt"); 

	print_to_file(file_data_rate_name, size_message, rate);

	char * file_data_rate_gnu_name[MAXLINE];

	memset(&file_data_rate_gnu_name, 0, sizeof(file_data_rate_gnu_name)); 
	strcat(file_data_rate_gnu_name, argv[1]);
	strcat(file_data_rate_gnu_name, s_message);
	strcat(file_data_rate_gnu_name, "_rate");  
	strcat(file_data_rate_gnu_name, "_gnu");
	strcat(file_data_rate_gnu_name, ".txt"); 
	
	memset(&file_data_rate_name, 0, sizeof(file_data_rate_name)); 
	
	strcat(file_data_rate_name, argv[1]);
	sprintf(s_message, "_%d", number_packages);
	strcat(file_data_rate_name, s_message);
	strcat(file_data_rate_name, "_rate");

	create_gnu_file(file_data_rate_gnu_name, file_data_rate_name);
*/

	return 0; 
}

struct xbee * configure_xbee(struct xbee *xbee, xbee_err ret)
{
	// Setup libxbee, using USB port to serial adapter
	// ttyUSBX at 9600 baud and check if errors
	if((ret = xbee_setup(&xbee, "xbeeZB", "/dev/ttyUSB1", 9600))== XBEE_ENONE)
		printf("Configuring xbee: OK\n");
	else
		printf("Configuring xbee: %s(Code: %d)\n", xbee_errorToStr(ret), ret);
	// Return xbee
	return xbee;
}

struct xbee_con * connection_xbee(struct xbee *xbee, struct xbee_con *con, xbee_err ret)
{
	// Address of the remote xbee
	struct xbee_conAddress address;

	memset(&address, 0, sizeof(address));

	address.addr64_enabled = 1;
	address.addr64[0] = 0x00;
	address.addr64[1] = 0x13;
	address.addr64[2] = 0xA2;
	address.addr64[3] = 0x00;
	address.addr64[4] = 0x41;
	address.addr64[5] = 0xC1;
	address.addr64[6] = 0xB9;
	address.addr64[7] = 0x54;

	// Create a new AT connection to the remote xbee
	if((ret = xbee_conNew(xbee, &con, "Data", &address))== XBEE_ENONE)
		printf("Connecting xbee: OK\n\n");
	else
		printf("Connecting xbee: %s(Code: %d)\n\n",
				xbee_errorToStr(ret), ret);
	// Return connection
	return con;
}


void * listen_server(void *arg){
	
	struct Server_ZigBee *server_zigBee;
	server_zigBee = (struct  Server_ZigBee  *) arg;

	//No more packages should be received than sent 
	while (count_packages_received < number_packages){ 
		receive_data(server_zigBee->xbee, server_zigBee->con, server_zigBee->ret);
		// char *message_json[MAXLINE];
		// memset(&message_json, 0, sizeof(message_json)); 

		// struct json_object *server_in;
		// struct json_object *client_out;
		// struct json_object *server_out;
		// struct json_object *data;

		// char current_time[MAXLINE]; 

		
		
		/*time_t client_in = time(NULL);
    	struct tm *lt = localtime(&client_in);
   		strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", lt);
		
		printf("\n\nPackage %d received. Current time: %s (TZ=%s)\n\n", i_package+1, current_time, "PST8PDT");	 
		
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
		
		// (times)[i_package] = time_travel;
		i_package++;*/
	
	}
	// printf("listen_server. \n");
}

void callback_function(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data)
{
	printf("callback_function\n");
	if(count_packages_received < number_packages){
	
	// Store data in buffer
	memset(read_buffer, '\0', BUFFER_SIZE);
	strcpy(read_buffer, (*pkt)->data);
	printf("read_buffer: %s\n",read_buffer);

	// Get current time and date
	// Initialize date/time struct
	time_t curtime;
	struct tm *timeinfo;
	
	// Get system current date/time
	time (&curtime);
	
	// Format date/time variable
	timeinfo = localtime(&curtime);
	
	// Get current date
	char dateString[20];
	strftime(dateString, sizeof(dateString)-1, "%Y-%m-%d", timeinfo);
	
	// Get current time
	char timeString[40];
	strftime(timeString, sizeof(timeString), "%H:%M:%S", timeinfo);
	
	// Create json object
	json_object *jobj = json_object_new_object();
	
	// Add date/time objects into main json object
	json_object *jdate = json_object_new_string(dateString);
	json_object_object_add(jobj, "Date", jdate);
	json_object *jtime = json_object_new_string(timeString);
	json_object_object_add(jobj, "Time", jtime);

	// Add the converted value to the json object
	json_object *jmoisture = json_object_new_string(&read_buffer[0]);
	json_object_object_add(jobj,"Data", jmoisture);
	
	// Copy the json string to the buffer
	strcpy(read_buffer, json_object_to_json_string(jobj));
	 
  	printf("JSON FORMAT: %s\n\n", &read_buffer[0]);
	count_packages_received++;
	
	}
}

void receive_data(struct xbee *xbee, struct xbee_con *con,xbee_err ret)
{
	// printf("package: %d -",count_packages_received);
	// printf("receive_data\n");
	
	// Associate data with a connection
	if ((ret = xbee_conDataSet(con, xbee, NULL)) != XBEE_ENONE)
		printf("Associating data error: %d\n\n", ret);
	// Configure a callback for the connection, this function
	// is called every time a packet for this connection is received
	if ((ret = xbee_conCallbackSet(con, callback_function, NULL))!= XBEE_ENONE)
		printf("Configuring callback: %s(Code: %d)\n\n",
				xbee_errorToStr(ret), ret);
}

void send_data(struct xbee *xbee, struct xbee_con *con,xbee_err ret)
{	
	printf("send_data\n");
	char message_json[MAXLINE];
	char time_[MAXLINE]; 

	// Associate data with a connection
	if ((ret = xbee_conDataSet(con, xbee, NULL)) == XBEE_ENONE)
		printf("Associating data: OK\n\n");
 
 	/*// Creating message
	// memset(&message_, 0, sizeof(message_));
	// for (int i = 0; i < size_message - 8; i++)
	// { 
	// 	strcat(message_, "x"); 
	// }
 	// memset(&message_json, 0, sizeof(message_json)); 
	// strcpy(message_json, "{ \"data\" :\"");
	// strcat(message_json, message_); 

	// time_t client_out = time(NULL);
	// strcat(message_json, "\", \"client_out\": "); 		
	// sprintf(time_, "%ld", client_out);
	// strcat(message_json, time_); 
		  
	// printf("message_json: %s \n", message_json);*/

	unsigned char retVal; 

	if ((ret = xbee_conTx(con, &retVal, "Hello World! it is %d in the CLIENT", 2021)) != XBEE_ENONE) {
        if (ret == XBEE_ETX) {
                fprintf(stderr, "a transmission error occured... (0x%02X)\n", retVal);
        } else {
                fprintf(stderr, "an error occured... %s\n", xbee_errorToStr(ret));
        } 
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
	fprintf(ou_file,"set terminal postscript landscape noenhanced color \\\n");
	fprintf(ou_file,"dashed defaultplex \"Helvetica\" 14\n");
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
	fprintf(ou_file,"plot \"%s.txt\" using 1:2 title \"Tasa de paquetes perdidos\" w lp pt 5 pi -4 lt rgb \"violet\", \\\n", file_data_name);
	fprintf(ou_file,"#w linespoint\n");
	fprintf(ou_file,"#    EOF\n");


   return 1;
}

