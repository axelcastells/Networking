#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include "VortexOnline.h"

void Protocol(Network::TCP::Server &server, sf::Packet& packet, int roomIndex, int socketIndex) {
	
}

int main()
{
	PlayerInfo playerInfo;
	TCP_SERVER.Run(Protocol, 50000, 4, true);
	//TCP_BTSSERVER.Run(5, 50001);

	while (1);
	return 0;
}