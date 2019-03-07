// TcpClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../udpClientLibrary/udpClient.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>

using namespace std;

#define IP_ADDRESS  "192.168.2.87"
#define PORT 50002

typedef struct {
	double Value1;
	double Value2;
}SEND_DATA;

udpClient theUdpReceiveClient(IP_ADDRESS, PORT, RECEIVE_FUNCTION);

int main() {

	int iResult;
	SEND_DATA data;
	char rcvbuf[sizeof(data) + 1];

	//	theUdpReceiveClient.connectToServer(IP_ADDRESS, PORT, RECEIVE_FUNCTION, RECEIVE_FUNCTION);

	do {
		iResult = theUdpReceiveClient.receiveFromServer(rcvbuf, sizeof(data) + 1);
		if (iResult == SOCKET_ERROR) {
			cout << "receive failed with error" << endl;
			while (cin.get() != '\n');
			cin.get();
			return 1;
		}
		cout << "Bytes Received: " << iResult << endl;

		memcpy(&data, &rcvbuf, sizeof(data));
		cout << "Receiving " << data.Value1 << " , " << data.Value2 << endl;
	} while (!_kbhit());

	theUdpReceiveClient.disconnectFromServer();
	cout << "Disconnected from server" << endl;

	while (cin.get() != '\n');
	cin.get();
	return 0;
}


