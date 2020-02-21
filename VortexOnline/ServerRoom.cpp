#pragma once
#include "ServerRoom.h"

ServerRoom::ServerRoom(int _maxClients)
{
	maxClients = _maxClients;
}

void ServerRoom::AddSocket(sf::TcpSocket *socketToBootstrapServer)
{
	if (sockets.size() <= maxClients) {
		sockets.push_back(socketToBootstrapServer);
	}
}

sf::TcpSocket & ServerRoom::PopSocket(int index)
{
	sf::TcpSocket &sock = *sockets[index];
	sockets.erase(sockets.begin() + index);
	return sock;
	// TODO: insertar una instrucción return aquí
}

sf::TcpSocket &ServerRoom::GetSocket(int index)
{
	return *sockets[index];
	// TODO: insertar una instrucción return aquí
}

int ServerRoom::GetSocketsCount()
{
	return sockets.size();
}

int ServerRoom::MaxClients()
{
	return maxClients;
}
