#pragma once
#include <winsock2.h>
#include <Ws2tcpip.h>

#define DEFAULT_BUFLEN 512

typedef enum {
	NO_FUNCTION = 0,
	RECEIVE_FUNCTION,
	SEND_FUNCTION
}TRANSMIT_FUNCTION;

class udpClient {

public:
	udpClient();
	udpClient(const char * ip_address, unsigned int port, TRANSMIT_FUNCTION transmit_function);

	int connectToServer(const char  *ip_address, unsigned int port, TRANSMIT_FUNCTION transmit_function);
	int sendToServer(const char * buf, int len);
	int receiveFromServer(char * buf, int len);
	int isConnectedToServer(void);
	int disconnectFromServer(void);

protected:
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct sockaddr_in RecvAddr = {}, SenderAddr = {};
	int SenderAddrSize = sizeof(SenderAddr);
	int connected = 0;
	TRANSMIT_FUNCTION function = NO_FUNCTION;
};