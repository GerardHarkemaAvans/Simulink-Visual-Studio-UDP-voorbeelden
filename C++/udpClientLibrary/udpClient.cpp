/*!
\file udpClient.cpp
\brief Implementatie(.cpp) van de UDP/IP library voor UDP/IP communicatie met een Arduino Simulink model
\author Gerard Harkema
\date 6 maart 2019
*/
#include "stdafx.h"  // Dummy to make it compiling
#include "udpClient.h"
#include <iostream>
#include <stdio.h>

//#define DBG_MESSAGE

using namespace std;

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

udpClient::udpClient() {

}
udpClient::udpClient(const char * ip_address, unsigned int port, TRANSMIT_FUNCTION transmit_function) {
	this->connectToServer(ip_address, port, transmit_function);
}

int udpClient::connectToServer(const char * ip_address, unsigned int port, TRANSMIT_FUNCTION transmit_function) {
	int iResult;

	//----------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
#if defined DBG_MESSAGE
		cout << "WSAStartup failed with error: " << iResult << endl;
#endif
		return iResult;
	}
#if defined DBG_MESSAGE
	cout << "WSAStartup: Oke\n";
#endif

	//----------------------
	// Create a SOCKET for connecting to server
	ConnectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ConnectSocket == INVALID_SOCKET) {
#if defined DBG_MESSAGE
		cout << "socket failed with error: " << WSAGetLastError() << endl;
#endif
		WSACleanup();
		return SOCKET_ERROR;
	}
#if defined DBG_MESSAGE
	cout << "Create socket Oke" << endl;
#endif
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(port);
	switch (transmit_function) {
	case RECEIVE_FUNCTION:
		RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		iResult = bind(ConnectSocket, (SOCKADDR *)& RecvAddr, sizeof(RecvAddr));
		if (iResult != 0) {
			wprintf(L"bind failed with error %d\n", WSAGetLastError());
			return SOCKET_ERROR;
		}
		break;
	case SEND_FUNCTION:
		inet_pton(AF_INET, ip_address, &RecvAddr.sin_addr.s_addr);
		break;
	default:
		return SOCKET_ERROR;
		break;
	}

	this->connected = ~0;
	this->function = transmit_function;
	return NO_ERROR;
}

int udpClient::sendToServer(const char * buf, int len) {

	if (this->function == RECEIVE_FUNCTION && this->connected){
		int iResult;
		iResult = sendto(this->ConnectSocket, buf, len, 0, (sockaddr*)&RecvAddr, sizeof(RecvAddr));

		if (iResult == SOCKET_ERROR) {
#if defined DBG_MESSAGE
			cout << "sendTo failed with error: " << WSAGetLastError() << endl;
#endif
			closesocket(ConnectSocket);
			WSACleanup();
			return SOCKET_ERROR;
		}
#if defined DBG_MESSAGE
		cout << "Bytes Sent: " << iResult << endl;
#endif
		return iResult;
	}
	else {
		return SOCKET_ERROR;
	}
}

int udpClient::receiveFromServer(char * buf, int len) {

	if (this->function == RECEIVE_FUNCTION && this->connected) {

		int iResult;
		iResult = recvfrom(ConnectSocket,
			buf, len, 0, (SOCKADDR *)& SenderAddr, &SenderAddrSize);
		if (iResult == SOCKET_ERROR) {
			wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
		}


		if (iResult < 0)
		{
#if defined DBG_MESSAGE
			cout << "receive failed with error: " << WSAGetLastError() << endl;
#endif
			return iResult;
		}
#if defined DBG_MESSAGE
		buf[len] = '\0';
		cout << "Bytes received: " << iResult << endl;
		cout << buf << endl;
#endif
		return iResult;
	}
	else {
		return SOCKET_ERROR;
	}
}

int udpClient::isConnectedToServer(void) {
	return this->connected;
}

int udpClient::disconnectFromServer(void) {
	// shutdown the connection since no more data will be sent
	int iResult;

	this->connected = 0;
	this->function = NO_FUNCTION;

	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
#if defined DBG_MESSAGE
		cout << "shutdown failed with error: " << WSAGetLastError() << endl;
#endif
		closesocket(ConnectSocket);
		WSACleanup();
		return iResult;
	}

	// close the socket
	iResult = closesocket(ConnectSocket);
	if (iResult == SOCKET_ERROR) {
#if defined DBG_MESSAGE
		cout << "close failed with error: " << WSAGetLastError() << endl;
#endif
		WSACleanup();
		return iResult;
	}

	WSACleanup();
	return NO_ERROR;
}
