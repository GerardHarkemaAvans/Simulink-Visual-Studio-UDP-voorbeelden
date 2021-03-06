// TcpClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../WifiClientLibrary/WifiClient.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>

using namespace std;

#define IP_ADDRESS  "192.168.2.87"
#define PORT 25000

typedef struct {
	double Value1;
	double Value2;
}SEND_DATA;

wifiClient theWifiClient(IP_ADDRESS, PORT);

int main() {

	int iResult;
	SEND_DATA data{ 0.255,0 };
	char sendbuf[sizeof(data)];

//	theWifiClient.connectToServer(IP_ADDRESS, PORT);

	do {
		cout << "Sending " << data.Value1 << " , " << data.Value2 << endl;
		memcpy(sendbuf, &data, sizeof(data));
		iResult = theWifiClient.sendToServer(sendbuf, sizeof(data));
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

	theWifiClient.disconnectFromServer();
	cout << "Disconnected from server" << endl;

	while (cin.get() != '\n');
	cin.get();

	return 0;
}
