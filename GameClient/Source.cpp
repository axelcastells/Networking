#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include "Graphics.h"





int main()
{
	PlayerInfo playerInfo;
	Graphics g;
	g.DrawDungeon();
	return 0;
}