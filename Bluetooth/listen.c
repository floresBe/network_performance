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
#include<bluetooth.h>

#define PORT     6050 
#define MAXLINE 1024 

// Driver code 
int main() {

	int ssock = 0;
    int csock = 0;
    int reuse_addr = 1;
    struct sockaddr_l2 src_addr;
    struct bt_security bt_sec;
    int result = 0;
	
	//Primero, necesitamos crear un socket, desde el cual podemos aceptar una conexión. 
	ssock = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
	
	// Queremos asegurarnos de que haya sido exitoso:
	if (ssock < 0) {
        perror("Opening L2CAP socket failed");
        return -1;
    }

	// Ahora tenemos que completar la estructura de la dirección de origen con una dirección de comodín, 
	// para que cualquier dispositivo Bluetooth con cualquier dirección pueda conectarse a nosotros.
	memset(&src_addr, 0, sizeof(src_addr));
    bacpy(&src_addr.l2_bdaddr, BDADDR_ANY);
    src_addr.l2_family = AF_BLUETOOTH;
    src_addr.l2_bdaddr_type = BDADDR_LE_PUBLIC;
    src_addr.l2_cid = htobs(CID_ATT);

	// La configuración de la opción SO_REUSEADDR nos permitirá 
	// llamar al enlace de nuevo rápidamente si es necesario (esto se puede omitir):
	setsockopt(ssock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));

	// A continuación, debemos vincular el socket con la estructura de dirección de origen que acabamos de definir.
	result = bind(ssock, (struct sockaddr*) &src_addr, sizeof(src_addr));
    if (result < 0) {
        perror("Binding L2CAP socket failed");
        return -1;
    }

	// Lo siguiente es establecer el nivel de seguridad. 
	memset(&bt_sec, 0, sizeof(bt_sec));
    bt_sec.level = BT_SECURITY_MEDIUM;
    result = setsockopt(ssock, SOL_BLUETOOTH, BT_SECURITY, &bt_sec, sizeof(bt_sec));
    if (result != 0) {
        perrorno("Setting L2CAP security level failed");
        return -1;
    }

	// Ahora podemos decirle al kernel que nuestro ssock es un socket pasivo, que aceptará una conexión.
	result = listen(ssock, 10);
    if (result < 0) {
        perror("Listening on L2CAP socket failed");
        return -1;
    }

	// Ahora podemos esperar una conexión entrante. 
	memset(peer_addr, 0, sizeof(*peer_addr));
    socklen_t addrlen = sizeof(*peer_addr);
    csock = accept(ssock, (struct sockaddr*)peer_addr, &addrlen);
    if (csock < 0) {
        perror("Accepting connection on L2CAP socket failed");
        return -1;
    }

	// Podemos imprimir la dirección del dispositivo conectado (opcional, por supuesto),
	// así como usar la función batostr para convertir la dirección Bluetooth en una cadena.
	printf("Accepted connection from %s", batostr(&peer_addr->l2_bdaddr));

	// Si no queremos que se conecten otros dispositivos, debemos cerrar el socket del servidor. 
	close(ssock);
    return csock;
}