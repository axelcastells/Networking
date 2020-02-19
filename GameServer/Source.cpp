#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include "Server.h"

int main()
{
	
	PlayerInfo playerInfo;
	Server::Run(5000, 4, false);

	while (1);
	return 0;
}