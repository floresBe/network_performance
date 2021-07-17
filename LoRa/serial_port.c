// C library headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Linux headers
#include <fcntl.h>  
#include <errno.h>  
#include <termios.h>  
#include <unistd.h>  

#define MAXLINE 1024

int size_message, number_packages, count_packages_received = 0;
char  USB_port_number[20]; 
int serial_port;
int travel_time;

void initialize_port(USB_port_number, msg);

void * timeout();
pthread_t time_thread;
pthread_mutex_t mutex_time_thread;
int time_out = 0;

int main(int argc, char *argv[]) {
  // Verifing args
	if (argc != 4) {
		fprintf(stderr,"It should be used like this: %s USB_port_number size_message number_packages\n", argv[0]);
		exit(1);
	}
 
	strcpy(USB_port_number, argv[1]); // /dev/ttyACM1
	size_message = atoi(argv[2]);
	number_packages = atoi(argv[3]);
 

  // message to write to serial port 
  char msg[10]; 
  memset(&msg, 0, sizeof(msg));  
  strcat(msg, argv[3]);
  strcat(msg, ";");
  strcat(msg, argv[2]); 

  initialize_port(USB_port_number, msg);

  	// Start thread to listen for server
	if (pthread_mutex_init(&mutex_time_thread, NULL) != 0) {
		perror("Failed to start mutex initialization");
		exit(EXIT_FAILURE);
	}
 
	pthread_create(&time_thread, NULL, timeout, NULL);

  
  
  //listen port
  while (count_packages_received < number_packages && !time_out)
  { 
    // pthread_mutex_lock(&mutex_time_thread);
    // if (!time_out){
      // pthread_mutex_unlock(&mutex_time_thread);

      char read_buf [256];
      memset(&read_buf, '\0', sizeof(read_buf));

      int num_bytes = read(serial_port, &read_buf, 4);
      // printf("num_bytes %d\n", num_bytes);

      if (num_bytes < 0) {
          printf("Error reading: %s", strerror(errno));
          return 1;
      }

      if(num_bytes > 0){ 

        printf("\nReceived message: %s\n", read_buf);
        count_packages_received++;

        travel_time += atoi(read_buf);
        // printf("time: %d\n", time);
      }
    // }
  }
  
  int jitter = travel_time / count_packages_received;
  printf("jitter: %d\n", jitter);

  int rate = abs((((float) count_packages_received/(float)number_packages)* 100)); 
  printf("rate: %d\n", rate);

  create_gnu_files(rate,jitter);
  
  close(serial_port);
  return 0; // success
}

void initialize_port(char  USB_port_number[20], char msg[10]){

// Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
  serial_port = open(USB_port_number, O_RDWR);

  // Create new termios struc, we call it 'tty' for convention
  struct termios tty;

  // Read in existing settings, and handle any error
  if(tcgetattr(serial_port, &tty) != 0) {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return;
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = 0;

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B9600);
  cfsetospeed(&tty, B9600);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return;
  }

  write(serial_port, msg, sizeof(msg));

}

void * timeout(){
  printf( "timeout %d\n", time_out);
  clock_t start_time;
	clock_t end_t;
	double seconds_elapsed = 0;
	start_time = clock(); 
	double timeout = number_packages * 1; //1 millisecond for every send message
  
  // Timeout wating
  while ( seconds_elapsed < timeout * 3000) {
		end_t = clock(); 
		seconds_elapsed = (double)(end_t - start_time) / CLOCKS_PER_SEC * 1000; 
		// printf( "[WAITING FOR %.0f Of %.0f MILISECONDS ]", seconds_elapsed, timeout * 3000);
		// printf( "\r");
	} 
  
  time_out = 1;
  printf( "timeout %d\n", time_out); 

}


int print_to_file(char * filename, int data1, float data2)
{
  printf("print_to_file\n");
   FILE * ou_file; 
   
   printf("filename %s\n",filename);
   ou_file = fopen(filename, "a+");
   
   if ( !ou_file ) {
      return 0;
   }
	
	fprintf(ou_file,"%d\t%.2f\n", data1, data2);

   return 1;
}

void create_gnu_files(float rate, float jitter){

	// file_name: USB_port_number + number_packages + jitter.txt
  printf("create_gnu_files\n");
	char s_message[MAXLINE];

	char * file_data_jitter_name[MAXLINE];

	memset(&s_message, 0, sizeof(s_message)); 

	memset(&file_data_jitter_name, 0, sizeof(file_data_jitter_name)); 
	strcat(file_data_jitter_name, "gnu_files/");
	strcat(file_data_jitter_name, "Arduino_Lora");
	sprintf(s_message, "_%d", number_packages);
	strcat(file_data_jitter_name, s_message);
	strcat(file_data_jitter_name, "_jitter");
	strcat(file_data_jitter_name, ".txt");

	print_to_file(file_data_jitter_name, size_message, jitter);


	// file_name: USB_port_number + number_packages + jitter + gnu.txt
	char * file_data_jitter_gnu_name[MAXLINE];

	memset(&file_data_jitter_gnu_name, 0, sizeof(file_data_jitter_gnu_name)); 

	strcat(file_data_jitter_gnu_name, "gnu_files/");
	strcat(file_data_jitter_gnu_name, "Arduino_Lora");
	strcat(file_data_jitter_gnu_name, s_message);
	strcat(file_data_jitter_gnu_name, "_jitter");  
	strcat(file_data_jitter_gnu_name, "_gnu");
	strcat(file_data_jitter_gnu_name, ".txt"); 
	
	memset(&file_data_jitter_name, 0, sizeof(file_data_jitter_name)); 
	
	strcat(file_data_jitter_name, "gnu_files/");
	strcat(file_data_jitter_name, "Arduino_Lora");
	sprintf(s_message, "_%d", number_packages);
	strcat(file_data_jitter_name, s_message);
	strcat(file_data_jitter_name, "_jitter");

	create_txt_file(file_data_jitter_gnu_name, file_data_jitter_name, "Jitter");

	// file_name: USB_port_number + number_packages + rate.txt
	char * file_data_rate_name[MAXLINE]; 

	memset(&file_data_rate_name, 0, sizeof(file_data_rate_name)); 

	strcat(file_data_rate_name, "gnu_files/");
	strcat(file_data_rate_name, "Arduino_Lora");
	sprintf(s_message, "_%d", number_packages);
	strcat(file_data_rate_name, s_message);
	strcat(file_data_rate_name, "_rate");  
	strcat(file_data_rate_name, ".txt"); 

	print_to_file(file_data_rate_name, size_message, rate);

	// file_name: USB_port_number + number_packages + jitrateter + gnu.txt
	char * file_data_rate_gnu_name[MAXLINE];

	memset(&file_data_rate_gnu_name, 0, sizeof(file_data_rate_gnu_name)); 

	strcat(file_data_rate_gnu_name, "gnu_files/");
	strcat(file_data_rate_gnu_name, "Arduino_Lora");
	strcat(file_data_rate_gnu_name, s_message);
	strcat(file_data_rate_gnu_name, "_rate");  
	strcat(file_data_rate_gnu_name, "_gnu");
	strcat(file_data_rate_gnu_name, ".txt"); 
	
	memset(&file_data_rate_name, 0, sizeof(file_data_rate_name)); 
	
	strcat(file_data_rate_name, "gnu_files/");
	strcat(file_data_rate_name, "Arduino_Lora");
	sprintf(s_message, "_%d", number_packages);
	strcat(file_data_rate_name, s_message);
	strcat(file_data_rate_name, "_rate");

	create_txt_file(file_data_rate_gnu_name, file_data_rate_name,"Tasa de paquetes perdidos");

}

int create_txt_file(char * filename, char * file_data_name, char desctiption[MAXLINE])
{
  printf("create_txt_file\n");
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