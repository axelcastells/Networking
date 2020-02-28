#include "Online.h"
#include <iostream>


using namespace Network;
#pragma region TCP

// ---------
// CLIENT 
// ---------

TCP::Client::Client()
{

}

TCP::Client &TCP::Client::Instance()
{
	static Client instance;
	return instance;
}







void TCP::Client::Run(void(*funcProtocol)(Client &_client, sf::Packet& packet), std::string _ip, short _port)
{
	FunctionProtocol = funcProtocol;
	ip = _ip;
	connectPort = _port;
	socketToBootstrapServer.connect(ip, connectPort);

	// Receive packets from Server. Use a thread.
	std::thread socketManager(&Client::ManageSocket, this);
	socketManager.detach();
}

void TCP::Client::Send(sf::Packet& packet)
{
	socketToBootstrapServer.send(packet);
}

void TCP::Client::ManageSocket()
{
	while (1)
	{
		sf::Packet packet;
		if (socketToBootstrapServer.receive(packet) == sf::Socket::Status::Done)
			FunctionProtocol(Instance(), packet);
	}
}

// -----------------------------------------
// SERVER
// -----------------------------------------



TCP::Server::Server()
{

}

void TCP::Server::Debugger()
{
	while (1) {
		system("cls");
		std::cout << "Unassigned Sockets [" << unassignedSockets.size() << "] " << std::endl;
		std::cout << "Rooms [" << serverRooms.size() << "] " << std::endl;
		for (int i = 0; i < serverRooms.size(); i++) {
			std::cout << "   " << "Room [" << i << "]: Sockets [" << serverRooms[i].GetSocketsCount() << " / " << serverRooms[i].MaxClients() << "]" << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

TCP::Server &TCP::Server::Instance()
{
	static Server instance;
	return instance;
}

void TCP::Server::Run(void(*funcProtocol)(Server &_server, sf::Packet& packet, int roomIndex, int socketIndex), short _port, int _maxRooms, bool debug)
{
	maxServerRooms = _maxRooms;
	connectPort = _port;
	FunctionProtocol = funcProtocol;

	if (listener.listen(connectPort) == sf::Socket::Status::Done) {
		socketSelector.add(listener);
	}

	std::thread manageSocketThread(&Server::ManageSockets, this);
	manageSocketThread.detach();
	if (debug) {
		std::thread debugger(&Server::Debugger, this);
		debugger.detach();

	}
}





int TCP::Server::CreateRoom(int maxClients) {
	serverRooms.push_back(ServerRoom(maxClients));
	return RoomsCount() - 1;
}

void TCP::Server::CloseRoom(int index)
{
	for (int i = 0; i < serverRooms[index].GetSocketsCount(); i++)
	{
		unassignedSockets.push_back(&serverRooms[index].PopSocket(i));
	}

	serverRooms.erase(serverRooms.begin() + index);
}

void TCP::Server::JoinRoom(int unassignedSocketIndex, int toRoomIndex)
{
	if (toRoomIndex < maxServerRooms) {
		serverRooms[toRoomIndex].AddSocket(unassignedSockets[unassignedSocketIndex]);
		unassignedSockets.erase(unassignedSockets.begin() + unassignedSocketIndex);
	}
}

void TCP::Server::ExitRoom(int roomIndex, int socketIndex) {
	sf::TcpSocket &sock = serverRooms[roomIndex].PopSocket(socketIndex);
	unassignedSockets.push_back(&sock);
}

int TCP::Server::RoomsCount() {
	return serverRooms.size();
}

ServerRoom & TCP::Server::GetRoom(int index)
{
	return serverRooms[index];
	// TODO: insertar una instrucción return aquí
}

void TCP::Server::Send(int sock, sf::Packet & packet)
{
	unassignedSockets[sock]->send(packet);
}

void TCP::Server::Send(int room, int sock, sf::Packet & packet)
{
	serverRooms[room].GetSocket(sock).send(packet);
}

void TCP::Server::BroadcastSend(int _roomIndex, sf::Packet & packet)
{
	for (int i = 0; i < serverRooms[_roomIndex].GetSocketsCount(); i++)
	{
		serverRooms[_roomIndex].GetSocket(i).send(packet);
	}
}

void TCP::Server::ManageSockets()
{
	while (1) {
		if (socketSelector.wait()) {
			if (socketSelector.isReady(listener) && serverRooms.size() <= maxServerRooms) {
				// The listener is ready: there is a pending connection
				sf::TcpSocket* client = new sf::TcpSocket;
				// Set socket blocking mode
				client->setBlocking(false);
				if (listener.accept(*client) == sf::Socket::Done)
				{
					// Add the new client to the clients list
					std::cout << "Llega el cliente con puerto: " << client->getRemotePort() << std::endl;
					unassignedSockets.push_back(client);
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
				for (int i = 0; i < unassignedSockets.size(); i++) {
					if (socketSelector.isReady(*unassignedSockets[i])) {
						sf::Packet packet;
						sf::Socket::Status status = unassignedSockets[i]->receive(packet);
						if (status == sf::Socket::Status::Done || status == sf::Socket::Status::Partial) {
							FunctionProtocol(Instance(), packet, -1, i);
						}
						else if (status == sf::Socket::Status::Disconnected)
						{
							unassignedSockets.erase(unassignedSockets.begin() + i);
						}

					}
				}

				for (int i = 0; i < serverRooms.size(); i++) {
					for (int j = 0; j < serverRooms[i].GetSocketsCount(); j++) {
						if (socketSelector.isReady(serverRooms[i].GetSocket(j))) {
							sf::Packet packet;
							sf::Socket::Status status = serverRooms[i].GetSocket(j).receive(packet);
							if (status == sf::Socket::Status::Done || status == sf::Socket::Status::Partial) {
								FunctionProtocol(Instance(), packet, i, j);
							}
							else if (status == sf::Socket::Status::Disconnected)
							{
								serverRooms[i].PopSocket(i);
							}
						}
					}
				}

			}
		}
	}
}

sf::TcpSocket & TCP::Server::LastSocket()
{
	return *unassignedSockets.back();
	// TODO: insertar una instrucción return aquí
}

int TCP::Server::LastSocketIndex()
{
	return unassignedSockets.size() - 1;
}

// --------------------------
// PEER
// --------------------------

TCP::Peer::Peer() {

}

TCP::Peer & TCP::Peer::Instance()
{
	static Peer instance;
	return instance;
	// TODO: insertar una instrucción return aquí
}

void TCP::Peer::Run(void(*funcProtocol)(Peer &_peer, sf::Packet &packet), std::string _ip, short _port, bool debug)
{
	FunctionProtocol = funcProtocol;
	ip = _ip;
	connectPort = _port;

	if (socketToBootstrapServer.connect(ip, connectPort) == sf::Socket::Status::Done) {
		socketSelector.add(socketToBootstrapServer);
		localPort = socketToBootstrapServer.getLocalPort();
	}

	std::thread peersManager(&Peer::ManagePeers, this);
	peersManager.detach();
	if (debug) {
		std::thread debugger(&Peer::Debugger, this);
		debugger.detach();

	}


}

void TCP::Peer::Send(int peerIndex, sf::Packet pack)
{
	sockets[peerIndex]->send(pack);
}

void TCP::Peer::ManagePeers()
{
	while (1) {
		if (socketSelector.wait()) {
			if (socketSelector.isReady(listener)) {
				// The listener is ready: there is a pending connection
				sf::TcpSocket* client = new sf::TcpSocket;
				// Set socket blocking mode
				//client->setBlocking(false);
				if (listener.accept(*client) == sf::Socket::Done)
				{
					std::cout << "Accepted new Peer!" << std::endl;
					std::cout << "IP: " << client->getRemoteAddress() << "SERVER_PORT: " << client->getRemotePort() << std::endl;
					socketSelector.add(*client);
					sockets.push_back(client);
				}

			}
			if (socketSelector.isReady(socketToBootstrapServer)) {
				sf::Packet packet;
				sf::Socket::Status stat = socketToBootstrapServer.receive(packet);



				if (stat == sf::Socket::Done) {

					std::cout << "Matchmaking Server Disconnected!" << std::endl;
					std::cout << directions.size() << std::endl;

					socketToBootstrapServer.disconnect();
					//listener.setBlocking(false);
					if (listener.listen(localPort) == sf::Socket::Status::Done) {
						socketSelector.add(listener);
						std::cout << "Me quedo en el " << localPort << std::endl;
					}



					std::cout << "Received Server Packet" << std::endl;

					std::string _ip;
					unsigned short _port;
					int numConnections;
					packet >> numConnections;

					for (int i = 0; i < numConnections; i++) {
						packet >> _ip >> _port;
						sf::TcpSocket* sock = new sf::TcpSocket();
						sock->setBlocking(true);
						if (sock->connect(_ip, _port) == sf::Socket::Done)
						{
							std::cout << "Added Socket: " << "IP: " << sock->getRemoteAddress() << "SERVER_PORT: " << sock->getRemotePort() << std::endl;
							socketSelector.add(*sock);
							sockets.push_back(sock);
						}
						std::cout << "Sockets: " << sockets.size() << std::endl;
					}
				}
				else
				{
					std::cout << "Error al recibir paquete" << std::endl;
				}
			}

			else {
				for (int i = 0; i < sockets.size(); i++) {
					if (socketSelector.isReady(*sockets[i])) {
						// Manage other peer socket
						sf::Packet pack;
						sf::Socket::Status status = sockets[i]->receive(pack);
						if (status == sf::Socket::Done) {
							FunctionProtocol(Instance(), pack);
						}
						else if (status == sf::Socket::Disconnected) {
							sockets.erase(sockets.begin() + i);
						}
					}
				}
			}
		}
	}
}
void TCP::Peer::Debugger()
{
	while (1) {
		system("cls");
		std::cout << "Peers [" << sockets.size() << "] " << std::endl;
		for (int i = 0; i < sockets.size(); i++) {
			std::cout << "   " << "IP: " << sockets[i]->getRemoteAddress() << "SERVER_PORT: " << sockets[i]->getRemotePort() << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
// ------------------------------
// Bootstrap Server
// ------------------------------
TCP::BootstrapServer::BootstrapServer() {

}

TCP::BootstrapServer & TCP::BootstrapServer::Instance()
{
	static BootstrapServer instance;
	return instance;
}

void TCP::BootstrapServer::Run(int _maxUsers, short _port)
{
	connectPort = _port;
	maxUsers = _maxUsers;

	if (listener.listen(connectPort) == sf::Socket::Status::Done) {
		socketSelector.add(listener);
	}

	std::thread manageSocketThread(&BootstrapServer::ManageSockets, this);
	//std::thread debugger(&Server::Debugger, this);
	manageSocketThread.detach();
	//debugger.detach();
}

void TCP::BootstrapServer::ManageSockets()
{
	bool runing = true;
	while (runing) {
		if (socketSelector.wait()) {
			if (socketSelector.isReady(listener) && directions.size() < maxUsers) {
				// The listener is ready: there is a pending connection
				sf::TcpSocket* client = new sf::TcpSocket;
				// Set socket blocking mode
				//client->setBlocking(false);
				if (listener.accept(*client) == sf::Socket::Done)
				{
					// Add the new client to the clients list
					std::cout << "Llega el cliente con puerto: " << client->getRemotePort() << std::endl;

					std::cout << "Connected new socket!" << std::endl;
					std::cout << "Socket count: " << directions.size() << std::endl;

					sf::Packet pack;
					pack << (int)directions.size();
					for (int i = 0; i < directions.size(); i++) {
						pack << directions[i].ip.toString() << directions[i].port;
					}

					directions.push_back(ConnectionData());
					directions[directions.size() - 1].ip = client->getRemoteAddress();
					directions[directions.size() - 1].port = client->getRemotePort();
					
					client->send(pack);
					//client->disconnect();
				}
				else
				{
					// Error, we won't get a new connection, delete the socket
					std::cout << "Error al recoger conexión nueva\n";
					delete client;
				}
			}
		}
	}
}

#pragma endregion


#pragma region UDP

UDP::Server::Server() {
	isRunning = false;
}

UDP::Server &UDP::Server::Instance()
{
	static Server instance;
	return instance;
}

void Network::UDP::Server::PongReceived(unsigned int _userId)
{
	connectionsById[_userId]->pingTime = 0;
}

void Network::UDP::Server::Send(sf::Packet _pack, ConnectionData dir)
{
	socketToBootstrapServer.send(_pack, dir.ip, dir.port);
}

void Network::UDP::Server::SendBroadcast(sf::Packet _pack, unsigned int _pingTime)
{
	for (auto i = connectionsById.begin(); i != connectionsById.end(); i++) {
		socketToBootstrapServer.send(_pack, i->second->ip, i->second->port);
		i->second->pingTime += _pingTime;
	}
}

void Network::UDP::Server::AddConnection(unsigned int newUid, ConnectionData dir)
{
	connectionsById[newUid] = new ConnectionData(dir);
}

// Returns through "proxy" arg the user proxy assigned to the given user id. You can use the returned boolean to check if there was a coincidence and therefore "proxy" has been written through reference.
bool Network::UDP::Server::GetConnectionData(unsigned int _userId, ConnectionData& dir)
{
	if (connectionsById.count(_userId)) {
		dir = *connectionsById[_userId];
		return true;
	}
	else {
		return false;
	}
}
bool Network::UDP::Server::GetConnectionId(const ConnectionData& proxy, unsigned int& _id)
{
	for (auto it = connectionsById.begin(); it != connectionsById.end(); it++) {
		if (*it->second == proxy) {
			_id = it->first;
			return true;
		}
	}
	return false;
}

unsigned int Network::UDP::Server::AddCriticalPacket(unsigned int _playerUid, sf::Packet _pack)
{
	unsigned int uid = NEW_UID;
	criticalPackets[uid] = Proxy(_pack, _playerUid);
	return uid;
}

void Network::UDP::Server::ResendCriticalPackets()
{
	for (auto i = criticalPackets.begin(); i != criticalPackets.end(); i++) {
		socketToBootstrapServer.send(i->second.packet, connectionsById[i->second.playerUid]->ip, connectionsById[i->second.playerUid]->port);
	}
}

void Network::UDP::Server::Debug()
{
	while (isRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		//system("cls");
		std::cout << "Connections (" << connectionsById.size() << ")" << std::endl;

		for (auto i = connectionsById.begin(); i != connectionsById.end(); i++) {
			std::cout << "	User ID: " << i->first << " IP: " << i->second->ip << ":" << i->second->port << " Ping: " << i->second->pingTime << std::endl;
		}

		std::cout << "|---|" << std::endl;
	}
}

void Network::UDP::Server::ManageSocketsThread()
{
	while (isRunning) {

		sf::Packet pack;
		ConnectionData dir;
		// FIND USER ID
		if (socketToBootstrapServer.receive(pack, dir.ip, dir.port) == sf::Socket::Status::Done) {
			FunctionProtocol(Instance(), dir, pack);
			//if (!connectionsByData.count(dir)){//connectionsByData[dir] == 0) {
			//	unsigned int uid = NEW_UID;
			//	AddConnection(uid, dir);
			//	FunctionProtocol(Instance(), uid, pack);

			//}
			//else {
			//	unsigned int uid = GetConnectionId(dir);
			//	FunctionProtocol(Instance(), connectionsByData[dir], pack);
			//}
		}

	}
}

void Network::UDP::Server::Ping()
{
	while (isRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(pingerMillis));
		SendBroadcast(pingPacket, pingerMillis);
	}
}

void Network::UDP::Server::ManageDisconnections()
{
	while (isRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(pingerMillis));

		std::vector<unsigned int> iterators;
		for (auto i = connectionsById.begin(); i != connectionsById.end(); i++) {

			if (i->second->pingTime >= disconnectPingCycles * pingerMillis) {
				iterators.push_back(i->first);
			}
		}
		for (int i = 0; i < iterators.size(); i++) {
			connectionsById.erase(iterators[i]);
		}
	}
}

void UDP::Server::Run(void(*funcProtocol)(Server &server, ConnectionData dir, sf::Packet& packet), sf::Packet _pingPacket, short _port, unsigned int criticTimer, unsigned int pingTime, unsigned int _disconnectPingCycles, bool debug) {
	
	disconnectPingCycles = _disconnectPingCycles;
	pingPacket = _pingPacket;
	criticalPacketMillis = criticTimer;
	pingerMillis = pingTime;
	port = _port;
	sf::Socket::Status status = socketToBootstrapServer.bind(port);
	if (status != sf::Socket::Done) {
		std::cout << "Can't bind port!" << std::endl;
	}
	FunctionProtocol = funcProtocol;
	
	isRunning = true;
	std::thread runServerThread(&Server::ManageSocketsThread, this);
	std::thread manageDisconnect(&Server::ManageDisconnections, this);
	std::thread managePingPong(&Server::Ping, this);
	std::thread manageCriticalPackets(&Server::CriticPacketManager, this);

	runServerThread.detach();
	manageDisconnect.detach();
	managePingPong.detach();
	manageCriticalPackets.detach();

	if (debug) {
		std::thread debugThread(&Server::Debug, this);
		debugThread.detach();
	}

	while (isRunning);
}

void UDP::Server::CriticPacketManager()
{
	while (isRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(criticalPacketMillis));
		ResendCriticalPackets();
	}
}

UDP::Client& UDP::Client::Instance()
{
	static Client instance;
	return instance;
}

void UDP::Client::Run(bool _debug)
{
	isRunning = true;
	std::thread mainThread(&UDP::Client::ManageSocket, this);
	mainThread.detach();
}

void Network::UDP::Client::Send(sf::Packet _packet)
{
	socketToBootstrapServer.send(_packet, serverIp, serverPort);
}

UDP::Client::Client() {
	isRunning = false;
}

void UDP::Client::ManageSocket() {
	while (isRunning) {
		sf::Packet packet;
		ConnectionData data;
		socketToBootstrapServer.receive(packet, data.ip, data.port);

		FunctionProtocol(Instance(), packet);
	}
}

void UDP::Client::Init(void(*funcProtocol)(Client &client, sf::Packet &_pack), sf::IpAddress _ip, unsigned short _serverPort, unsigned short _localPort)
{
	FunctionProtocol = funcProtocol;
	serverIp = _ip;
	serverPort = _serverPort;

	socketToBootstrapServer.bind(_localPort);
}

#pragma endregion