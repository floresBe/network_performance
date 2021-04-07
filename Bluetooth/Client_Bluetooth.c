#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/sco.h>
#include <json-c/json.h>
#include <sys/socket.h>

#define MAXLINE 1024

void send_data(int s);
void * listen_server(void *arg); 
void callback_function(char buf[MAXLINE]);
int create_array(int **array, int c);
float get_average();
void create_gnu_files(float rate, float jitter, char device_name[MAXLINE]);
int create_txt_file(char * filename, char * file_data_name, char desctiption[MAXLINE]);

pthread_t thread_listen_server;
pthread_mutex_t mutex_packages_received;

int size_message, number_packages, count_packages_received = 0;
int ** times;

struct listen_server_struct
{
    int s;
    struct sockaddr_rc address;
};

int main(int argc, char **argv)
{ 
	if (argc != 4) {
		fprintf(stderr,"It should be used like this: %s device_name size_message number_packages\n", argv[0]);
		exit(1);
	}
    
    int flag=0;

    struct sockaddr_rc address = { 0 };
    int s, status;
    char device_name[20]; //="flores";

    strcpy(device_name, argv[1]); 
	size_message = atoi(argv[2]);
	number_packages = atoi(argv[3]);

    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    address.rc_family = AF_BLUETOOTH;
    address.rc_channel = (uint8_t) 1;//must use sdp to work in real devices 

    printf("Search for BT Device\n");

    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;

    char addr[18] = { 0 };
    char name[248] = { 0 };


	memset(&times, 0, sizeof(times)); 
	if ( !create_array(&times, number_packages)) {
      fprintf(stderr,"Error: Unable to create array\n");
      return 0;
   	}

    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

    len = 8;
    max_rsp = 2;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
    
    // BT Search
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) {
        perror("hci_inquiry");
    }

    for (i = 0; i < num_rsp; i++) {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0) {    
            strcpy(name, "[unknown]");
        }

        int a=strcmp(name, device_name);
    
        if (!a){   // if name mached 
            str2ba( addr, &address.rc_bdaddr );   
            flag =1;
        }
    }//End Search
     
    //Connect
    if (flag==0){
        printf("Not find dest: %s\n",name);
        exit(0);
    }
    
    status = connect(s, (struct sockaddr *)&address, sizeof(address));
    
    if( status != 0 ) {
        if( status < 0 ){ 
            perror("send message to server Failed\n");
            exit(0);
        }
    }

    struct listen_server_struct listen_server_struct;
    listen_server_struct.address = address;
    listen_server_struct.s = s;  

    if (pthread_mutex_init(&mutex_packages_received, NULL) != 0) {
		perror("Failed to start mutex initialization");
		exit(EXIT_FAILURE);
	}
    
    //thread begins 
	pthread_create(&thread_listen_server, NULL, listen_server, (void *)&listen_server_struct);

    for (size_t i = 0; i < number_packages; i++)
    {   
        send_data(s);
        printf("package %d send\n\n", i);
		sleep(1);
    }
 
	// sleep(1);
	// int timeout = number_packages * 0.5; 
	// // Timeout wating
	// printf("[ WAITING FOR %d SECONDS ]\n",timeout);
	// printf("\n");
	// sleep(timeout);

	printf("\n");
    
    int p_cancel = pthread_cancel(thread_listen_server);
    if (p_cancel != 0)
        perror("pthread_cancel");

    printf("Closing socket\n");
    close(s);
    close( sock );

    float jitter ;
	float rate;

	memset(&jitter, 0, sizeof(jitter)); 
	memset(&rate, 0, sizeof(rate)); 
	
    jitter = get_average();
	rate = abs((((float) count_packages_received/(float)number_packages)* 100) - 100);

	printf("\n\nPackages sent: %d\n", number_packages);
	printf("Packages received: %d\n", count_packages_received);
	printf("Rate of lost packets: %.0f \n", rate );
	printf("Jitter: %.2f\n", jitter);
	
	create_gnu_files(rate,jitter, device_name);

    return 0;
}

void send_data(int s)
{	 
	char message_json[MAXLINE];
	char time_[MAXLINE]; 
	char message_[72];
	
 	// Creating json message
	memset(&message_, 0, sizeof(message_));
	for (int i = 0; i < size_message; i++)
	{ 
		strcat(message_, "x"); 	
	}
 	memset(&message_json, 0, sizeof(message_json)); 
	strcpy(message_json, "{\"a\":\"");
	strcat(message_json, message_); 

	time_t client_out = time(NULL);
	strcat(message_json, "\",\"b\":"); 		
	sprintf(time_, "%ld", client_out);
	strcat(message_json, time_); 
		  
	write(s, message_json, 72);
}

void * listen_server(void *arg)
{	struct listen_server_struct *listen_server_struct;
	listen_server_struct = (struct  listen_server_struct  *) arg;

	listen(listen_server_struct->s, 1);
    char buf[1024] = { 0 };
    int bytes_read;
    socklen_t opt = sizeof(listen_server_struct->address);

    ba2str( &listen_server_struct->address.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));
    
    while (count_packages_received < number_packages){ 
        bytes_read = read(listen_server_struct->s, buf, sizeof(buf));
      
        if( bytes_read > 0 ) {
            printf("bytes_read [%d]\n", bytes_read);
            printf("received [%s]\n", buf);
            callback_function(buf);
        }
    }
}

void callback_function(char buf[MAXLINE])
{
	// printf("callback_function\n");
	if(count_packages_received < number_packages){
		
		pthread_mutex_lock(&mutex_packages_received);
		// Get current time
		time_t curtime;
		struct tm *timeinfo;
		time (&curtime);
 
		printf("package %d received \n\n", count_packages_received);

		char *message_json[MAXLINE];
		struct json_object *parsed_json;
		struct json_object *server_in;
		struct json_object *client_out;
		struct json_object *server_out;
		struct json_object *data;

		memset(&message_json, 0, sizeof(message_json)); 

		parsed_json = json_tokener_parse(buf);
		
		json_object_object_get_ex(parsed_json, "a", &data);
		json_object_object_get_ex(parsed_json, "b", &client_out);
		json_object_object_get_ex(parsed_json, "c", &server_in);
		json_object_object_get_ex(parsed_json, "d", &server_out);

		int client_out_int, server_in_int, server_out_int, client_in_int;
		
		client_out_int = json_object_get_int(client_out);
		server_in_int = json_object_get_int(server_in);
		server_out_int = json_object_get_int(server_out);
		client_in_int = (long int) curtime;

		int time_travel;
		memset(&time_travel, 0, sizeof(time_travel)); 
		time_travel = (server_in_int - client_out_int) + (client_in_int - server_out_int);

		printf("server_in: %d\n", server_in_int);
		printf("client_out: %d\n", client_out_int);
		printf("client_in: %d\n", client_in_int);
		printf("server_out: %d\n\n", server_out_int);

		times[count_packages_received] = time_travel;
		printf("time_travel: %d\n\n", time_travel);

		count_packages_received++;
		pthread_mutex_unlock(&mutex_packages_received);
	}
}

int create_array(int **array, int c)
{
   (*array) = (int *) malloc(c * sizeof(int));
   if ( !(*array) ) {
      return 0;
   }
   return 1;
}

float get_average(){
	float avrg = 0; 
	
	pthread_mutex_lock(&mutex_packages_received);
	for (int i = 0; i < count_packages_received-1; i++)
	{  
		int difference = (times)[i + 1] - (times)[i];  
		avrg += (float) difference;
	} 

	avrg = avrg/(float)count_packages_received; 
	pthread_mutex_unlock(&mutex_packages_received);
	return avrg;
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

void create_gnu_files(float rate, float jitter, char device_name[MAXLINE]){

	// file_name: device_name + number_packages + jitter.txt
    // printf("create_gnu_files \n");
	char s_message[MAXLINE];

	char * file_data_jitter_name[MAXLINE];

	memset(&s_message, 0, sizeof(s_message)); 

	memset(&file_data_jitter_name, 0, sizeof(file_data_jitter_name)); 
	strcat(file_data_jitter_name, "gnu_files/");
	strcat(file_data_jitter_name, device_name);
	sprintf(s_message, "_%d", number_packages);
	strcat(file_data_jitter_name, s_message);
	strcat(file_data_jitter_name, "_jitter");
	strcat(file_data_jitter_name, ".txt");

	print_to_file(file_data_jitter_name, size_message, jitter);


	// file_name: device_name + number_packages + jitter + gnu.txt
	char * file_data_jitter_gnu_name[MAXLINE];

	memset(&file_data_jitter_gnu_name, 0, sizeof(file_data_jitter_gnu_name)); 

	strcat(file_data_jitter_gnu_name, "gnu_files/");
	strcat(file_data_jitter_gnu_name, device_name);
	strcat(file_data_jitter_gnu_name, s_message);
	strcat(file_data_jitter_gnu_name, "_jitter");  
	strcat(file_data_jitter_gnu_name, "_gnu");
	strcat(file_data_jitter_gnu_name, ".txt"); 
	
	memset(&file_data_jitter_name, 0, sizeof(file_data_jitter_name)); 
	
	strcat(file_data_jitter_name, "gnu_files/");
	strcat(file_data_jitter_name, device_name);
	sprintf(s_message, "_%d", number_packages);
	strcat(file_data_jitter_name, s_message);
	strcat(file_data_jitter_name, "_jitter");

	create_txt_file(file_data_jitter_gnu_name, file_data_jitter_name, "Jitter");

	// file_name: device_name + number_packages + rate.txt
	char * file_data_rate_name[MAXLINE]; 

	memset(&file_data_rate_name, 0, sizeof(file_data_rate_name)); 

	strcat(file_data_rate_name, "gnu_files/");
	strcat(file_data_rate_name, device_name);
	sprintf(s_message, "_%d", number_packages);
	strcat(file_data_rate_name, s_message);
	strcat(file_data_rate_name, "_rate");  
	strcat(file_data_rate_name, ".txt"); 

	print_to_file(file_data_rate_name, size_message, rate);

	// file_name: device_name + number_packages + jitrateter + gnu.txt
	char * file_data_rate_gnu_name[MAXLINE];

	memset(&file_data_rate_gnu_name, 0, sizeof(file_data_rate_gnu_name)); 

	strcat(file_data_rate_gnu_name, "gnu_files/");
	strcat(file_data_rate_gnu_name, device_name);
	strcat(file_data_rate_gnu_name, s_message);
	strcat(file_data_rate_gnu_name, "_rate");  
	strcat(file_data_rate_gnu_name, "_gnu");
	strcat(file_data_rate_gnu_name, ".txt"); 
	
	memset(&file_data_rate_name, 0, sizeof(file_data_rate_name)); 
	
	strcat(file_data_rate_name, "gnu_files/");
	strcat(file_data_rate_name, device_name);
	sprintf(s_message, "_%d", number_packages);
	strcat(file_data_rate_name, s_message);
	strcat(file_data_rate_name, "_rate");

	create_txt_file(file_data_rate_gnu_name, file_data_rate_name,"Tasa de paquetes perdidos");
}

int create_txt_file(char * filename, char * file_data_name, char desctiption[MAXLINE])
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
	fprintf(ou_file,"set ylabel \"%s (%c) \" \n",desctiption, '%');
	fprintf(ou_file,"set key left top\n");
	fprintf(ou_file,"set title \"%s en funcion del tamano del buffer UDP\" \n", desctiption);
	fprintf(ou_file,"#set xrange [ 0 : 130 ] noreverse nowriteback\n");
	fprintf(ou_file,"#set yrange [ -.5 : 14 ] noreverse nowriteback\n");
	fprintf(ou_file,"#set mxtics 5.000000\n");
	fprintf(ou_file,"#set mytics 1.000000\n");
	fprintf(ou_file,"#set xtics border mirror norotate 1\n");
	fprintf(ou_file,"#set ytics border mirror norotate 0.5\n");
	fprintf(ou_file,"plot \"%s.txt\" using 1:2 title \"%s\" w lp pt 5 pi -4 lt rgb \"violet\", \\\n", file_data_name, desctiption);
	fprintf(ou_file,"#w linespoint\n");
	fprintf(ou_file,"#    EOF\n");


   return 1;
}

