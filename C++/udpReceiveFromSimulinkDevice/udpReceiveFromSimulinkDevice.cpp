/**
\file udpReceiveFromSimulinkDevice.cpp
\brief Voorbeeld programma om gegevens uit een Arduino simulink-model te ontvangen
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
#define PORT 50002

/*! SEND_DATA: Definitie van de data-structuur die wordt verzonden naar het simulink model
Deze moet overeenkomen met de "Data size"van het simulink blok
*/
typedef struct {
	double Value1;
	double Value2;
}SEND_DATA;

/*! \brief Het object waardoor de data wordt onvangen, tevens wordt de verbinding tot stand gebracht */
udpClient theUdpReceiveClient(IP_ADDRESS, PORT, RECEIVE_FUNCTION);

/*! \brief Beginpunt van het programma */
int main() {

	int iResult;
	SEND_DATA data;
	char rcvbuf[sizeof(data) + 1];

	//	theUdpReceiveClient.connectToServer(IP_ADDRESS, PORT, RECEIVE_FUNCTION, RECEIVE_FUNCTION); // Alternatieve methode

	do {
		// Lees informatie van de server(Arduino)
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
	} while (!_kbhit());// Is keyboard toets ingedrukt ?

	// Verbreek de verbinding
	theUdpReceiveClient.disconnectFromServer();
	cout << "Disconnected from server" << endl;

	while (cin.get() != '\n');
	cin.get();
	return 0;
}


