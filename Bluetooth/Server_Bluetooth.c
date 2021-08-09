#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#define MAXLINE 1024 
void callback_function(char buf[MAXLINE], int client);

int main(int argc, char **argv)
{
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    listen(s, 1);
    printf("listenning\n");

    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    printf("accept\n");


   while (1)
   {
       printf("while\n");
       
    
        ba2str( &rem_addr.rc_bdaddr, buf);
        fprintf(stderr, "Accepted connection from %s\n", buf);
    
        memset(buf, 0, sizeof(buf));
        bytes_read = read(client, buf, sizeof(buf));
        printf("read\n");
    
        if( bytes_read > 0 ) {
            printf("received [%s]\n", buf);
            callback_function(buf, client);
        }
       
   }
     close(client);
    close(s);
    return 0;
}

void callback_function(char buf[MAXLINE], int client)
{ 
	time_t curtime;
	struct tm *timeinfo;
	
	time (&curtime); 
	time_t server_in = time(NULL);
	char current_time[MAXLINE];
	struct tm *lt = localtime(&server_in);

	time_t server_out = time(NULL);
	char time_[128];

	strcat(buf, ",\"c\":"); 
	sprintf(time_, "%ld", server_in);
	strcat(buf, time_); 
	
	strcat(buf, ",\"d\":"); 
	sprintf(time_, "%ld", server_out);
	strcat(buf, time_); 
	
	strcat(buf, "}"); 
	printf("%s\n", buf);

    printf("sended [%s]\n", buf);
    write(client, buf, 72);
}
