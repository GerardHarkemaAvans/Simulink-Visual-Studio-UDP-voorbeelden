// TcpClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../udpClientLibrary/udpClient.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>

using namespace std;

#define IP_ADDRESS  "192.168.2.87"
#define PORT 50001

typedef struct {
	double Value1;
	double Value2;
}SEND_DATA;

udpClient theUdpSendClient;// (IP_ADDRESS, PORT, SEND_FUNCTION);

int main() {

	int iResult;
	SEND_DATA data{ 0.255,0 };
	char sendbuf[sizeof(data)];

	iResult = theUdpSendClient.connectToServer(IP_ADDRESS, PORT, SEND_FUNCTION);
	if (iResult != NO_ERROR) {
		cout << "unable to connect to server" << endl;
	}

	do {
		cout << "Sending " << data.Value1 << " , " << data.Value2 << endl;
		memcpy(sendbuf, &data, sizeof(data));
		iResult = theUdpSendClient.sendToServer(sendbuf, sizeof(data));
		if (iResult == SOCKET_ERROR) {
			cout << "send failed with error" << endl;
			while (cin.get() != '\n');
			cin.get();
			return 1;
		}
		printf("Bytes Sent: %d\n", iResult);
		data.Value1++;
		data.Value2 = data.Value1 * 100;
		Sleep(100);
	}while (!_kbhit());

	theUdpSendClient.disconnectFromServer();
	cout << "Disconnected from server" << endl;

	while (cin.get() != '\n');
	cin.get();

	return 0;
}
