#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include "Graphics.h"
#include "VortexOnline.h"


//void Protocol(Network::TCP::Client &client, sf::Packet &packet) {
//
//}
void Protocol(Network::TCP::Peer &peer, sf::Packet &packet) {
	//Switch amb els diferents tipus de packets
}

int main()
{
	TCP_PEER.Run(Protocol, "localhost", 50001);
	PlayerInfo playerInfo;
	Graphics g;
	//g.DrawDungeon();
	while (1);
	return 0;
}