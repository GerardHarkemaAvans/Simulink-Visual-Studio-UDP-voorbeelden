/*!
\file udpClient.h
\brief Interface(.h) van de UDP/IP library voor UDP/IP communicatie met een Arduino Simulink model
\author Gerard Harkema
\date 6 maart 2019
*/
#pragma once
#include <winsock2.h>
#include <Ws2tcpip.h>

#define DEFAULT_BUFLEN 512

/*! \brief Definitie van de mogelijke verbindingstypes voor het object van de klasse udpClient\n

*/
typedef enum {
	NO_FUNCTION = 0,	/** Geen gedefineerde functie */
	RECEIVE_FUNCTION,	/** Configureer object voor ontvangen */
	SEND_FUNCTION		/** Configureer object voor zenden */
}TRANSMIT_FUNCTION;

class udpClient {

public:
	/*! Default constructor, maakt nog geen verbinding met het Arduino Simulink model.

	*/
	udpClient();

	/*! \brief Verbinding maken met de UDP/IP server in het Arduino Simulink model.\n

	\param ip_address	Ip address van UDP/IP server in her Arduino Simulink model
	\param port			Poortnummer van UDP/IP server in her Arduino Simulink model
	\param transmit_function	Type van het object ongedefineerd, als ontvanger of als zender
	<> 0: als er een fout is opgetreden.
	*/
	udpClient(const char * ip_address, unsigned int port, TRANSMIT_FUNCTION transmit_function);

	/*! \brief Verbinding maken met de UDP/IP server in het Arduino Simulink model.\n

	\param ip_address	Ip address van UDP/IP server in her Arduino Simulink model
	\param port			Poortnummer van UDP/IP server in her Arduino Simulink model
	\param transmit_function	Type van het object ongedefineerd, als ontvanger of als zender
	\return				0: bij succes\n
						<> 0: als er een fout is opgetreden.
	*/
	int connectToServer(const char  *ip_address, unsigned int port, TRANSMIT_FUNCTION transmit_function);
	
	/*! \brief Bericht verzenden naar het Arduino Simulink model.

	\param buf	pointer naar het te verzenden bericht
	\param len			lengte van het bericht in bytes
	\return				>=0: aantal verzonden bytes\n
	<0: fout bij het verzenden
	*/	
	int sendToServer(const char * buf, int len);
	
	/*! \brief Bericht ontvangen van het Arduino Simulink model.

	\param buf	pointer naar het te ontvangen bericht
	\param len			maximale lengte van het te ontvangen bericht in bytes
	\return				>=0: aantal ontvangen bytes in het bericht/n
	<0: fout bij het verzenden
	*/
	int receiveFromServer(char * buf, int len);
	
	/*! \brief Opvragen van de status van de verbinding

	\return				0: niet verbonden\n
	<>0: Verbonden
	*/	
	int isConnectedToServer(void);

	/*! \brief Verbinding verbreken met het Arduino Simulink model

	\return				0: bij succes \n
	<>0: als er een fout is opgetreden.
	*/	
	int disconnectFromServer(void);

	//! DOXYS_OFF
protected:
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct sockaddr_in RecvAddr = {}, SenderAddr = {};
	int SenderAddrSize = sizeof(SenderAddr);
	int connected = 0;
	TRANSMIT_FUNCTION function = NO_FUNCTION;
	//! DOXYS_ON
};