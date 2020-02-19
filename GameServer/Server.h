#pragma once
#include <SFML/Network.hpp>

namespace Server {
	static void(*FunctionProtocol)(sf::Packet& packet, int roomIndex, int socketIndex);
	void DebugConsole();
	void Run(int port, int maxServerRooms, bool debug);
}