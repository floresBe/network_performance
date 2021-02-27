// Server side implementation of ZigBee client-server model 
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
#include <xbee.h>
 
#define BUFFER_SIZE 256
#define SEND_TIME 10
#define MAXLINE 1024 

// Declare functions
struct xbee * configure_xbee(struct xbee *xbee, xbee_err ret);
struct xbee_con * connection_xbee(struct xbee *xbee, struct xbee_con *con, xbee_err ret);
void receive_data(struct xbee *xbee, struct xbee_con *con, xbee_err ret);
void callback_function(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data);

// Buffer to store the data received
char read_buffer[MAXLINE];

// Driver code 
int main() {
    // Xbee errors variable
	xbee_err ret;

	// Configure xbee
	struct xbee *xbee = configure_xbee(xbee, ret);
    unsigned char retVal; 
	
	// Create a new connection
	struct xbee_con *con = connection_xbee(xbee, con, ret);

	setenv("TZ", "PST8PDT", 1); // set time zone
	printf("Set time zone\n");
	printf("Listening...\n");
	
	// Receive data from the remote xbee
	while (1)
	{ 
		receive_data(xbee, con, ret);
		sleep(SEND_TIME);
	}

	// Shutdown connection
	xbee_conEnd(con);
	// Shutdown libxbee
	xbee_shutdown(xbee);

	return 0;
}

struct xbee * configure_xbee(struct xbee *xbee, xbee_err ret)
{
	// Setup libxbee, using USB port to serial adapter
	// ttyUSBX at 9600 baud and check if errors
	if((ret = xbee_setup(&xbee, "xbeeZB", "/dev/ttyUSB0", 9600))== XBEE_ENONE)
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
	address.addr64[6] = 0xAE;
	address.addr64[7] = 0xD1;

	// Create a new connection to the remote xbee
	if((ret = xbee_conNew(xbee, &con, "Data", &address))== XBEE_ENONE)
		printf("Connecting xbee: OK\n\n");
	else
		printf("Connecting xbee: %s(Code: %d)\n\n",
				xbee_errorToStr(ret), ret);

	// Return connection
	return con;
}

void receive_data(struct xbee *xbee, struct xbee_con *con,xbee_err ret)
{
	// Associate data with a connection
	if ((ret = xbee_conDataSet(con, xbee, NULL)) != XBEE_ENONE)
		printf("Associating data: OK\n");
	// Configure a callback for the connection, this function
	// is called every time a packet for this connection is received
	if ((ret = xbee_conCallbackSet(con, callback_function, NULL))!= XBEE_ENONE)
		printf("Configuring callback: %s(Code: %d)\n",
				xbee_errorToStr(ret), ret);
}

void callback_function(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data)
{
	xbee_err ret;
	unsigned char retVal; 

	// Store data in buffer
	memset(read_buffer, '\0', BUFFER_SIZE);
	strcpy(read_buffer, (*pkt)->data);
 
	time_t curtime;
	struct tm *timeinfo;
	
	// Get system current date/time
	time (&curtime); 
	time_t server_in = time(NULL);
	char current_time[MAXLINE];
	struct tm *lt = localtime(&server_in);

	time_t server_out = time(NULL);
	char time_[128];

	strcat(read_buffer, ",\"c\":"); 
	sprintf(time_, "%ld", server_in);
	strcat(read_buffer, time_); 
	
	strcat(read_buffer, ",\"d\":"); 
	sprintf(time_, "%ld", server_out);
	strcat(read_buffer, time_); 
	
	strcat(read_buffer, "}"); 
	printf("%s\n", &read_buffer[0]);

	// Transmit a message // maximum 72 characters
	if ((ret = xbee_connTx(con, &retVal, &read_buffer[0], 72) ) != XBEE_ENONE) {
         if (ret == XBEE_ETX) {
                 fprintf(stderr, "A transmission error occured... (0x%02X)\n", retVal);
        } else {
                fprintf(stderr, "An error occured... %s\n", xbee_errorToStr(ret));
        }
	} 
}
