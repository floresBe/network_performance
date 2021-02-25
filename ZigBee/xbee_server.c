/* Standard input/output definitions */
#include <stdio.h>
/* Standard General Utilities Library */
#include <stdlib.h>
/* File Control Definitions */
#include <fcntl.h>
/* UNIX Standard Definitions */
#include <unistd.h>
/* ERROR Number Definitions */
#include <errno.h>
/* String function definitions */
#include <string.h>
/* POSIX Terminal Control Definitions */
#include <termios.h>
/* Date and time manipulation */
#include <time.h>
/* Parser-JSON */
#include <json-c/json.h>
/* Digi's XBee wireless modules definitions */
#include <xbee.h>

// Buffer size
#define BUFFER_SIZE 256
// Send time in sec
#define SEND_TIME 10


// Declare functions
struct xbee * configure_xbee(struct xbee *xbee, xbee_err ret);
struct xbee_con * connection_xbee(struct xbee *xbee, struct xbee_con *con, xbee_err ret);
void receive_data(struct xbee *xbee, struct xbee_con *con, xbee_err ret);

void callback_function(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data);

// Create buffer to store the data received
char read_buffer[BUFFER_SIZE];

void main()
{
	// Xbee errors variable
	xbee_err ret;
	// Configure xbee
	struct xbee *xbee = configure_xbee(xbee, ret);
	// Create a new connection
	struct xbee_con *con = connection_xbee(xbee, con, ret);
	// Receive data from the remote xbee
	// and send it to altairsmartcore
	while(1) {
    xbee_conTx(con, NULL, "Hello\r\n");
		receive_data(xbee, con, ret);
		sleep(SEND_TIME);
	}
	// Shutdown connection
	xbee_conEnd(con);
	// Shutdown libxbee
	xbee_shutdown(xbee);
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
	address.addr64[5] = 0x84;
	address.addr64[6] = 0xAA;
	address.addr64[7] = 0xB4;

	// Create a new AT connection to the remote xbee
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

/* Callback function, it will be executed once for each packet
that is received on an associated connection */
void callback_function(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data)
{
	// Store data in buffer
	memset(read_buffer, '\0', BUFFER_SIZE);
	strcpy(read_buffer, (*pkt)->data);
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
	json_object_object_add(jobj,"Datos", jmoisture);
	// Copy the json string to the buffer
	strcpy(read_buffer, json_object_to_json_string(jobj));

  printf("JSON FORMAT: %s\n", &read_buffer[0]);
	// Transmit a message
	xbee_conTx(con, NULL, "Receiving data: OK");
}
