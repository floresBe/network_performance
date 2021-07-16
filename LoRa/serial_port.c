// C library headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Linux headers
#include <fcntl.h>  
#include <errno.h>  
#include <termios.h>  
#include <unistd.h>  

int size_message, number_packages, count_packages_received = 0;
char  USB_port_number[20]; 
int serial_port;
int time;

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
  
  //listen port
  while (count_packages_received < number_packages)
  { 
    char read_buf [256];
    memset(&read_buf, '\0', sizeof(read_buf));

    int num_bytes = read(serial_port, &read_buf, 4);
    // printf("num_bytes %d\n", num_bytes);
    
    if (num_bytes < 0) {
        printf("Error reading: %s", strerror(errno));
        return 1;
    }

    if(num_bytes > 0){ 
      
      printf("Received message: %s\n", read_buf);
      count_packages_received++;

      time += atoi(read_buf);
      // printf("time: %d\n", time);
    }
  }

  int jitter = time / count_packages_received;
  printf("jitter: %d\n", jitter);

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