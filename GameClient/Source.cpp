#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include "Graphics.h"
#include "VortexOnline.h"
#include "Protocol.h"


void Protocol(Network::TCP::Client &client, sf::Packet &packet) {

	//Switch amb els diferents tipus de packets rebuts

	int intHead;
	packet >> intHead;
	Header head = (Header)intHead;

	//Només missatges rebuts, respostes al main.
	switch (head)
	{
	case START:
		
		break;

	case UPDATE_POSITIONS:

		break;

	case YOUR_TURN:

		break;

	case MOVE:

		break;

		//Permet fer una demanda al servidor quan arribi a una sala
	case GUESS:

		break;

		//Rebre el supose d'un altre client
	case SUPOSE:

		break;

		//Rebre el acuse d'un altre client
	case ACUSE:

		break;

		//Rebre la notificació de qui guanya la partida i acabar
	case ACUSE_SUCCESS:

		break;

		//Rebre la notificacio de qui falla i queda eliminat
	case ACUSE_FAIL:

		break;

		//El client ensenya la carta escollida al client pertinent a traves del servidor.
	case SHOW_CARDS:

		break;

		//Llegir quin client esta ensenyant cartes a un altre
	case SHOWING_CARDS:

		break;

	}
}


/*void Protocol(Network::TCP::Peer &peer, sf::Packet &packet) {
	//Switch amb els diferents tipus de packets
	

	//packet.receivePacket >> header;

	switch (Header)
	{
	case START :
		break;

	case UPDATE_POSITIONS:
		break;

	case YOUR_TURN:
		break;

	case MOVE:
		break;

	case GUESS:
		break;

	case SUPOSE:
		break;

	case ACUSE:
		break;

	case ACUSE_SUCCESS:
		break;

	case ACUSE_FAIL:
		break;

	case SHOW_CARDS:
		break;

	case SHOWING_CARDS:
		break;

	}
}
*/

int main()
{
	//Client local setup
	PlayerInfo playerInfo;
	sf::TcpSocket serverSocket;
	sf::Packet loginPacket;
	bool startGame = false;
	int maxPlayers;
	int currentPlayersJoined;
	std::vector<PlayerInfo> players;
	std::vector<std::string> packets;

	// INIT PLAYER
	std::string nickname;
	std::cout << "Enter your nickname: ";
	std::cin >> nickname;
	loginPacket << "JOINED " << nickname;
	playerInfo.SetName(nickname);
	
	//Connect a Servidor
	TCP_CLIENT.Run(Protocol, "localhost", 50001);

	//Partida
	while (!startGame)
	{
		//Esperant jugadors a la partida


	};

	Graphics g;
	g.DrawDungeon();
	
	
	return 0;
}