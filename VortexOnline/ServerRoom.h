#pragma once
#pragma once
#include <SFML\Network.hpp>

class ServerRoom {
public:

	template<class T>
	void SetParameter(std::string tag, T param)
	{
		parameters[tag] = param;
	}

	template<class T>
	T GetParameter(std::string tag)
	{
		return (T)parameters[tag];
	}


	ServerRoom(int _maxClients);
	void AddSocket(sf::TcpSocket *socket);
	sf::TcpSocket &PopSocket(int index);
	sf::TcpSocket &GetSocket(int index);
	int GetSocketsCount();
	int MaxClients();
private:
	int maxClients;
	std::vector<sf::TcpSocket*> sockets;

	std::map<std::string, std::string> parameters;
};