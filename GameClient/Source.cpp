#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include "Graphics.h"
#include "VortexOnline.h"

enum Header { START, UPDATE_POSITIONS, YOUR_TURN, MOVE, GUESS, SUPOSE, ACUSE, ACUSE_SUCCESS, ACUSE_FAIL, SHOW_CARDS, SHOWING_CARDS };

void Protocol(Network::TCP::Client &client, sf::Packet &packet) {

	//Receive del packet

	//Switch amb els diferents tipus de packets

	switch (Header)
	{
	case START:
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
	//CONNECT + SETUP
	TCP_CLIENT.Run(Protocol, "localhost", 50001);
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
	
	Graphics g;
	g.DrawDungeon();
	while (1);
	
	return 0;
}