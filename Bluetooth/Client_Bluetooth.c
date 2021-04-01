#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/sco.h>
#include <sys/socket.h>

int main(int argc, char **argv)
{
    int flag=0;

    struct sockaddr_rc addrress = { 0 };
    int s, status;
    char device_name[20]="flores";

    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    addrress.rc_family = AF_BLUETOOTH;
    addrress.rc_channel = (uint8_t) 1;//must use sdp to work in real devices 

    printf("Search for BT Devices...\n");

    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;

    char addr[18] = { 0 };
    char name[248] = { 0 };

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

        // printf("\nFind #%d\n",i);
        // printf("Addr:%s    Name:%s\n", addr, name);
        
        int a=strcmp(name, device_name);
        // printf("compare:%d\n",a);
        
        if (!a){   // if name mached 
            str2ba( addr, &addrress.rc_bdaddr );   
            flag =1;
        }
    }
    // End Search 

    //Connect and send

    if (flag==0){
        printf("Not find dest: %s\n",name);
        exit(0);
    }

    // connect to server, throw socket s
    status = connect(s, (struct sockaddr *)&addrress, sizeof(addrress));
    //successful, connect() returns 0.

    // printf("connection status: %d\n\n",status);//0 show OK

    // send a message to server
    if( status != 0 ) {
        if( status < 0 ){ 
            perror("send message to server Failed\n");
            exit(0);
        }
    }  
    status = write(s, "hello from client", 6);
    if (status == 6){
        printf("Send data to server done\n");
    }
    
    listen(s, 1);
    char buf[1024] = { 0 };
    int bytes_read;
    int client = accept(s, (struct sockaddr *)&addrress, sizeof(addrress));
     ba2str( &addrress.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);


    memset(buf, 0, sizeof(buf));

    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));

    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
    }

    printf("Closing socket\n");
    close(s);

    free( ii );
    close( sock );

    return 0;
}
