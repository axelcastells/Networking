#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

#define DEBUG 0
#define MAX_SERVER_ROOMS 4
#define PORT 50000


namespace Server {
	void(*FunctionProtocol)(sf::Packet& packet, int roomIndex, int socketIndex);
	void DebugConsole();
	void Run();
}