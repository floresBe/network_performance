// Compile using "gcc -o scanner 1.c -lbluetooth" command

// System 
#include <stdio.h>
#include <stdlib.h>

// Bluetooth
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int
main() {

  // Get bluetooth device id
  int device_id = hci_get_route(NULL); // Passing NULL argument will retrieve the id of first avalaibe device 
  if (device_id < 0) {
    printf("Error: Bluetooth device not found");
    exit(1);
  }  

  // Get device Information
  struct hci_dev_info device_info;
  if (hci_devinfo(device_id, &device_info) == 0) {
    char device_address[18];
    ba2str(&device_info.bdaddr, device_address);
    printf("Local Device: %s  %s\n", device_address, device_info.name);
  }
 
  // Find nearby devices
  int len     = 8;                 // Search time = 1.28 * len seconds
  int max_rsp = 255;               // Return maximum max_rsp devices
  int flags   = IREQ_CACHE_FLUSH;  // Flush out the cache of previously detected devices.
  inquiry_info* i_infs = (inquiry_info*) malloc(max_rsp * sizeof(inquiry_info)); 
  int num_rsp = hci_inquiry(device_id, len, max_rsp, NULL, &i_infs, flags); // search  
  if (num_rsp < 0) {
    printf("Error: the hci_inquiry fails");
    exit(1);
  }
  printf("Found %d remote device\n", num_rsp); 

  // Open socket
  int socket = hci_open_dev(device_id);
  if (socket < 0) {
    printf("Error: Cannot open socket");
    exit(1);
  }

  int i;
  for (i = 0; i < num_rsp; i++) {
    inquiry_info* remote_device = i_infs + i;
    char          remote_device_address[18];
    char          remote_device_name[300];

    // Get HEX address
    ba2str(&(remote_device->bdaddr), remote_device_address);
    
    // Get device name
    if (hci_read_remote_name(socket, &(remote_device->bdaddr), sizeof(remote_device_name), remote_device_name, 0) < 0) {
        strcpy(remote_device_name, "[unknown]"); // If cannot retrieve the name then set it "unknown"
    }
    // print address and name
    printf("Remote Device: %s  %s\n", remote_device_address, remote_device_name);
  }  

  // Close the socket
  close(socket);

  // Remove allocated memory
  free(i_infs);
  
  return 0;
}