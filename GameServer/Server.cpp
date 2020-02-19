#pragma once
#include <iostream>
#include <thread>
#include "Server.h"

sf::TcpListener listener;
sf::SocketSelector socketSelector;
std::vector<std::vector<sf::TcpSocket*>> sockets;
static int serverRooms = 0;

namespace Server {
	void DebugConsole() {
		while (1) {
			system("cls");
			if (sockets.size() > 0) {
				std::cout << "Unassigned Sockets [" << sockets[0].size() << "] " << std::endl;
				std::cout << "Rooms [" << sockets.size() - 1 << "] " << std::endl;
				for (int i = 1; i < sockets.size(); i++) {
					std::cout << "   " << "Room [" << i << "]: Sockets [" << sockets[i].size() << " / " << sockets[i].size() << "]" << std::endl;
				}
			}
			else {
				std::cout << "Empty Server" << std::endl;
			}

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
	void ManageSockets() {
		while (1) {
			if (socketSelector.wait()) {
				if (socketSelector.isReady(listener) && sockets.size() - 1 <= serverRooms) {
					// The listener is ready: there is a pending connection
					sf::TcpSocket* client = new sf::TcpSocket;
					// Set socket blocking mode
					client->setBlocking(false);
					if (listener.accept(*client) == sf::Socket::Done)
					{
						// Add the new client to the clients list
						std::cout << "Llega el cliente con puerto: " << client->getRemotePort() << std::endl;
						sockets[0].push_back(client);
						// Add the new client to the selector so that we will
						// be notified when he sends something
						socketSelector.add(*client);
					}
					else
					{
						// Error, we won't get a new connection, delete the socket
						std::cout << "Error al recoger conexión nueva\n";
						delete client;
					}

				}
				else
				{
					//Sockets sin sala
					for (int i = 0; i < sockets[0].size(); i++) {
						if (socketSelector.isReady(*sockets[0][i])) {
							sf::Packet packet;
							sf::Socket::Status status = sockets[0][i]->receive(packet);
							if (status == sf::Socket::Status::Done || status == sf::Socket::Status::Partial) {
								FunctionProtocol(packet, 0, i);
							}
							else if (status == sf::Socket::Status::Disconnected)
							{
								sockets[0].erase(sockets[0].begin() + i);
							}

						}
					}

					//Salas de sockets
					for (int i = 1; i < sockets.size() - 1; i++) {
						for (int j = 0; j < sockets[i].size(); j++) {
							if (socketSelector.isReady(*sockets[i][j])) {
								sf::Packet packet;
								sf::Socket::Status status = sockets[i][j]->receive(packet);
								if (status == sf::Socket::Status::Done || status == sf::Socket::Status::Partial) {
									FunctionProtocol(packet, i, j);
								}
								else if (status == sf::Socket::Status::Disconnected)
								{
									sockets[i].erase(sockets[i].begin() + j);
								}
							}
						}
					}

				}
			}
		}
	}
	void Run(int port, int maxServerRooms, bool debug)
	{
		serverRooms = maxServerRooms;
		if (listener.listen(port) == sf::Socket::Status::Done) {
			socketSelector.add(listener);
		}

		std::thread manageSocketThread(ManageSockets);
		manageSocketThread.detach();

		if (debug) {
			std::thread debugger(&DebugConsole);
			debugger.detach();
		}
	}
}
