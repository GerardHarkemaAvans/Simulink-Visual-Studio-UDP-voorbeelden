/**
\file udpSendToSimulinkDevice.cpp
\brief Voorbeeld programma om gegevens naar een Arduino simulink-model te zenden
door middel van het udp/ip protocol
\author Gerard Harkema
\date 6 maart 2019
\remarks Gebuik dit programma samen met het TransmitToUdpServer.xls simulink model
*/


#include "stdafx.h"
#include "../udpClientLibrary/udpClient.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>

using namespace std;

/*! IP_ADDRESS: Ip address van het simulink model in de Arduino
*/
#define IP_ADDRESS  "192.168.2.87"

/*! PORT: poortnummer van het simulink model in de Arduino
*/
#define PORT 50001

/** SEND_DATA: Definitie van de data-structuur die wordt verzonden naar het simulink model
Deze moet overeenkomen met de "Data size"van het simulink blok
*/
typedef struct {
	double Value1;
	double Value2;
}SEND_DATA;

/*! \brief Het object waardoor de data wordt verzonden */
udpClient theUdpSendClient;

/*! \brief Beginpunt van het programma */
int main() {

	int iResult;
	SEND_DATA data{ 0.255,0 };
	char sendbuf[sizeof(data)];

	// Breng de verbinding tot stand
	iResult = theUdpSendClient.connectToServer(IP_ADDRESS, PORT, SEND_FUNCTION);
	if (iResult != NO_ERROR) {
		cout << "unable to connect to server" << endl;
	}

	do {
		// Zend informatie naar de server(Arduino)
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
	}while (!_kbhit());// Is keyboard toets ingedrukt ?

	// Verbreek verbinding
	theUdpSendClient.disconnectFromServer();
	cout << "Disconnected from server" << endl;

	while (cin.get() != '\n');
	cin.get();

	return 0;
}
