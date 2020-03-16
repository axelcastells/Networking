#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include "VortexOnline.h"
#include "Protocol.h"

void Protocol(Network::TCP::Server &server, sf::Packet& packet, int roomIndex, int socketIndex) {
	
	// Switch amb els diferents missatges a enviar o rebre dels clients?

	int intHead;
	packet >> intHead;
	Header head = (Header)intHead;

	switch (head)
	{
		//Enviar als clients que comença la partida
	case START:

		break;

		//Enviar posicions dels jugadors a tots els clients
	case UPDATE_POSITIONS:

		break;

		//Enviar al client que li toqui el torn
	case YOUR_TURN:

		break;

		
	case MOVE:

		break;

		//
	case GUESS:

		break;

		//
	case SUPOSE:

		break;

		//
	case ACUSE:

		break;

		//El client que fa la acusacio encerta i acaba la partida.
	case ACUSE_SUCCESS:

		break;

		//El client que fa la acusacio falla i es eliminat
	case ACUSE_FAIL:

		break;

		//
	case SHOW_CARDS:

		break;

		//
	case SHOWING_CARDS:

		break;

	}
}

int main()
{
	PlayerInfo playerInfo;
	TCP_SERVER.Run(Protocol, 50000, 4, true);
	bool startGame = false;
	int maxPlayers;
	int currentPlayersJoined;

	//Partida
	while (!startGame)
	{
		//Esperant jugadors a la partida


	};
	return 0;
}