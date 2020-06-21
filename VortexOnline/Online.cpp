#include "Online.h"
#include <iostream>
#include <random>


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

void TCP::Peer::BroadcastSend(sf::Packet pack) {
	for (int i = 0; i < sockets.size(); i++)
	{
		sockets[i]->send(pack);
	}
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
	manageSocketThread.detach();
}

void TCP::BootstrapServer::ManageSockets()
{
	bool isRunning = true;
	while (isRunning) {
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
	_pack << UDP_SYSTEM_MESSAGE::END_OF_PACKET;
	int packetMessageTag;
	_pack >> packetMessageTag;
	sf::Packet newPacket;
	newPacket << packetMessageTag << serverSalt;
	
	auto nextMessage = 0;
	while (nextMessage != UDP_SYSTEM_MESSAGE::END_OF_PACKET) {
		_pack >> nextMessage;
		newPacket << nextMessage;
	}

	socket.send(newPacket, dir.ip, dir.port);
}

void Network::UDP::Server::SendBroadcast(sf::Packet _pack, unsigned int _pingTime)
{
	mutex.lock();
	for (auto i = connectionsById.begin(); i != connectionsById.end(); i++) {
		Send(_pack, ConnectionData(i->second->ip, i->second->port));
		//socket.send(_pack, i->second->ip, i->second->port);
		i->second->pingTime += _pingTime;
	}
	mutex.unlock();
}

void Network::UDP::Server::AddConnection(unsigned int newUid, ConnectionData dir)
{
	mutex.lock();
	connectionsById[newUid] = new ConnectionData(dir);
	mutex.unlock();
}

// Returns through "proxy" arg the user proxy assigned to the given user id. You can use the returned boolean to check if there was a coincidence and therefore "proxy" has been written through reference.
bool Network::UDP::Server::GetConnectionData(unsigned int _userId, ConnectionData** dir)
{
	mutex.lock();
	if (connectionsById.count(_userId)) {
		*dir = connectionsById[_userId];
		mutex.unlock();
		return true;
	}
	else {
		mutex.unlock();
		return false;
	}
}
bool Network::UDP::Server::GetConnectionId(const ConnectionData& proxy, unsigned int& id)
{
	mutex.lock();
	for (auto it = connectionsById.begin(); it != connectionsById.end(); it++) {
		if (*it->second == proxy) {
			id = it->first;

			mutex.unlock();
			return true;
		}
	}
	mutex.unlock();
	return false;
}

unsigned int Network::UDP::Server::AddCriticalPacket(ConnectionData dir, sf::Packet _pack) {
	unsigned int uid = NEW_UID;
	mutex.lock();
	nonMemberCriticalPackets[uid] = std::pair<ConnectionData, sf::Packet>(dir, _pack);
	mutex.unlock();
	return uid;
}

unsigned int Network::UDP::Server::AddCriticalPacket(unsigned int _playerUid, sf::Packet _pack)
{
	unsigned int uid = NEW_UID;
	mutex.lock();
	criticalPackets[uid] = Proxy(_pack, _playerUid);
	mutex.unlock();
	return uid;
}

void Network::UDP::Server::RemoveCriticalPacket(unsigned int _criticalPacketId) {
	criticalPackets.erase(_criticalPacketId);
}

void Network::UDP::Server::RemoveNonMemberCriticalPacket(unsigned int _nonMemberCriticalPacketId) {
	nonMemberCriticalPackets.erase(_nonMemberCriticalPacketId);
}

void Network::UDP::Server::SendCriticalPackets()
{
	for (auto i = criticalPackets.begin(); i != criticalPackets.end(); i++) {
		mutex.lock();
		Send(i->second.packet, ConnectionData(connectionsById[i->second.playerUid]->ip, connectionsById[i->second.playerUid]->port));
		//socket.send(i->second.packet, connectionsById[i->second.playerUid]->ip, connectionsById[i->second.playerUid]->port);
		mutex.unlock();
	}

	for (auto i = nonMemberCriticalPackets.begin(); i != nonMemberCriticalPackets.end(); i++) {
		mutex.lock();
		Send(i->second.second, ConnectionData(i->second.first.ip, i->second.first.port));
		//socket.send(i->second.second, i->second.first.ip, i->second.first.port);
		mutex.unlock();
	}
}

void Network::UDP::Server::Debug()
{
	while (isRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		//system("cls");
		std::cout << "Connections (" << connectionsById.size() << ")" << std::endl;

		mutex.lock();
		for (auto i = connectionsById.begin(); i != connectionsById.end(); i++) {
			std::cout << "	User ID: " << i->first << " IP: " << i->second->ip << ":" << i->second->port << " Ping: " << i->second->pingTime << std::endl;
		}
		mutex.unlock();

		std::cout << "|---|" << std::endl;
	}
}

void Network::UDP::Server::ManageSocketsThread()
{
	while (isRunning) {

		sf::Packet pack;
		ConnectionData dir;
		// FIND USER ID
		if (socket.receive(pack, dir.ip, dir.port) == sf::Socket::Status::Done) {
			
			unsigned int clientSalt;

			sf::Packet potentialSystemPacket = pack;
			int potentialSystemMessageId;
			potentialSystemPacket >> potentialSystemMessageId;

			unsigned int newUid;
			if (potentialSystemMessageId == (int)UDP_SYSTEM_MESSAGE::HELLO) {
				//TODO: Manage salt stuff
				potentialSystemPacket >> clientSalt;
				
				if (!GetConnectionId(dir, newUid)) {
					sf::Packet challengePacket;
					challengePacket << (int)UDP_SYSTEM_MESSAGE::CHALLENGE_ID << (int)UDP_SYSTEM_MESSAGE::CHALLENGE_QUESTION;
					challengePacketId = AddCriticalPacket(dir, challengePacket);
				}
			}
			else if (potentialSystemMessageId == (int)UDP_SYSTEM_MESSAGE::CHALLENGE_ID) {
				potentialSystemPacket >> clientSalt;
				int challengeAnswer;
				potentialSystemPacket >> challengeAnswer;

				if (challengeAnswer == (int)UDP_SYSTEM_MESSAGE::CHALLENGE_ANSWER) {
					RemoveNonMemberCriticalPacket(challengePacketId);
					
					std::cout << "Correct challenge answer!" << std::endl;
					dir.salt = (serverSalt & clientSalt);

					newUid = NEW_UID;
					AddConnection(newUid, dir);

					std::cout << "Added new connection with id [" << newUid << "] and salt checksum [" << dir.salt << "]" << std::endl;

					sf::Packet welcomePacket;
					welcomePacket << (int)UDP_SYSTEM_MESSAGE::HELLO;
					Send(welcomePacket, dir);
				}


			}
			//If true, this is a ping packet!
			else if (potentialSystemMessageId == (int)UDP_SYSTEM_MESSAGE::PING) {
				potentialSystemPacket >> clientSalt;

				unsigned int id;
				GetConnectionId(dir, id);
				ConnectionData* data;

				if (GetConnectionData(id, &data)) {
					if ((clientSalt & serverSalt) == data->salt) {
						std::cout << "Got PONG from client!" << std::endl;
						//ConnectionData* proxy = nullptr;
						//GetConnectionData(newUid, &proxy);
						PongReceived(id);
						//proxy->pingTime = 0;
					}

				}

			}
			else {
				potentialSystemPacket >> clientSalt;

				unsigned int id;
				if (GetConnectionId(dir, id)) {
					ConnectionData* data;
					GetConnectionData(id, &data);

					if ((clientSalt & serverSalt) == data->salt)
					{
						//pack << potentialSystemMessageId << potentialSystemPacket;

						pack << UDP_SYSTEM_MESSAGE::END_OF_PACKET;
						sf::Packet newPacket;
						newPacket << potentialSystemMessageId;

						auto nextMessage = 0;
						while (nextMessage != UDP_SYSTEM_MESSAGE::END_OF_PACKET) {
							pack >> nextMessage;
							newPacket << nextMessage;
						}

						FunctionProtocol(Instance(), dir, newPacket);
					}
				}
			}
		}		
	}	
}

void Network::UDP::Server::Ping()
{
	while (isRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(pingerMillis));
		sf::Packet newPacket;
		newPacket << (int)UDP_SYSTEM_MESSAGE::PING;
		SendBroadcast(newPacket, pingerMillis);
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

void UDP::Server::Run(void(*funcProtocol)(Server &server, ConnectionData dir, sf::Packet& packet), short _port, unsigned int criticTimer, unsigned int pingTime, unsigned int _disconnectPingCycles, bool debug) {
	
	std::cout << "[UDP SERVER] Insert a bunch of random numbers ( /!\ ONLY NUMBERS /!\ ) and press ENTER." << std::endl;
	std::cin >> serverSalt;

	disconnectPingCycles = _disconnectPingCycles;
	criticalPacketMillis = criticTimer;
	pingerMillis = pingTime;
	port = _port;
	sf::Socket::Status status = socket.bind(port);
	if (status != sf::Socket::Done) {
		std::cout << "Can't bind port!" << std::endl;
	}
	FunctionProtocol = funcProtocol;
	
	isRunning = true;
	std::thread runServerThread(&Server::ManageSocketsThread, this);
	std::thread manageDisconnect(&Server::ManageDisconnections, this);
	std::thread managePingPong(&Server::Ping, this);
	std::thread manageCriticalPackets(&Server::CriticalPacketsManager, this);

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

void UDP::Server::CriticalPacketsManager()
{
	while (isRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(criticalPacketMillis));
		SendCriticalPackets();
	}
}

UDP::Client& UDP::Client::Instance()
{
	static Client instance;
	return instance;
}


void Network::UDP::Client::Send(sf::Packet _packet)
{
	_packet << UDP_SYSTEM_MESSAGE::END_OF_PACKET;
	int packetMessageTag;
	_packet >> packetMessageTag;
	sf::Packet newPacket;
	newPacket << packetMessageTag << clientSalt;

	auto nextMessage = 0;
	while (nextMessage != UDP_SYSTEM_MESSAGE::END_OF_PACKET) {
		_packet >> nextMessage;
		newPacket << nextMessage;
	}

	socketToBootstrapServer.send(newPacket, serverIp, serverPort);
}

UDP::Client::Client() {
	isRunning = false;
}

void UDP::Client::Pong()
{
	sf::Packet pongPacket;
	pongPacket << (int)UDP_SYSTEM_MESSAGE::PING;
	Send(pongPacket);
}

unsigned int Network::UDP::Client::AddCriticalPacket(sf::Packet _pack)
{
	unsigned int uid = NEW_UID;
	_pack << UDP_SYSTEM_MESSAGE::END_OF_PACKET;
	int packetMessageTag;
	_pack >> packetMessageTag;
	sf::Packet newPacket;
	newPacket << packetMessageTag << clientSalt;

	auto nextMessage = 0;
	while (nextMessage != UDP_SYSTEM_MESSAGE::END_OF_PACKET) {
		_pack >> nextMessage;
		newPacket << nextMessage;
	}
	criticalPackets[uid] = newPacket;
	return uid;
}

void Network::UDP::Client::RemoveCriticalPacket(unsigned int _criticalPacketId) {
	criticalPackets.erase(_criticalPacketId);
}

void UDP::Client::SendCriticalPackets() {
	for (auto i = criticalPackets.begin(); i != criticalPackets.end(); i++) {
		Send(i->second);
	}
}

void UDP::Client::CriticalPacketsManager()
{
	while (isRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(criticalPacketMillis));
		SendCriticalPackets();
	}
}

void UDP::Client::ManageSocket() {
	while (isRunning) {
		sf::Packet packet;
		ConnectionData data;
		if (socketToBootstrapServer.receive(packet, data.ip, data.port) == sf::Socket::Status::Done) 
		{
			unsigned int serverSalt;
			int potentialSystemId;
			sf::Packet potentialSystemPacket;
			potentialSystemPacket = packet;
			potentialSystemPacket >> potentialSystemId;

			//If true, this is a ping packet!
			if (potentialSystemId == (int)UDP_SYSTEM_MESSAGE::HELLO) {
				potentialSystemPacket >> serverSalt;
				std::cout << "Got WELCOME from server!" << std::endl;
				saltChecksum = (clientSalt & serverSalt);
			}
			else if (potentialSystemId == (int)UDP_SYSTEM_MESSAGE::CHALLENGE_ID) {
				RemoveCriticalPacket(helloPacketId);
				potentialSystemPacket >> serverSalt;
				int challengeQuestion;
				potentialSystemPacket >> challengeQuestion;
				std::cout << "Server challenging Client..." << std::endl;
				sf::Packet answerPacket;
				answerPacket << (int)UDP_SYSTEM_MESSAGE::CHALLENGE_ID << (int)UDP_SYSTEM_MESSAGE::CHALLENGE_ANSWER;
				Send(answerPacket);
			}
			else if (potentialSystemId == (int)UDP_SYSTEM_MESSAGE::PING) {
				potentialSystemPacket >> serverSalt;
				if ((serverSalt & clientSalt) == saltChecksum) {
					std::cout << "Got PING from server!" << std::endl;
					Pong();
				}
			}
			else {
				//TODO: CONFIRM IT'S A REAL CLIENT BEFORE LAUNCHING FUNCTION PROTOCOL
				potentialSystemPacket >> serverSalt;
				if ((serverSalt & clientSalt) == saltChecksum) {
					//packet << potentialSystemId << potentialSystemPacket;
					packet << UDP_SYSTEM_MESSAGE::END_OF_PACKET;
					sf::Packet newPacket;
					newPacket << potentialSystemId;

					auto nextMessage = 0;
					while (nextMessage != UDP_SYSTEM_MESSAGE::END_OF_PACKET) {
						packet >> nextMessage;
						newPacket << nextMessage;
					}

					FunctionProtocol(Instance(), newPacket);
				}
			}
			
		}
	}
}

void UDP::Client::Run(void(*funcProtocol)(Client &client, sf::Packet &_pack), sf::IpAddress _ip, unsigned short _serverPort, unsigned int _criticalPacketMillis)
{
	criticalPacketMillis = _criticalPacketMillis;
	FunctionProtocol = funcProtocol;
	serverIp = _ip;
	serverPort = _serverPort;

	
	socketToBootstrapServer.bind(sf::IpAddress::getLocalAddress().toInteger());

	sf::Packet helloPacket;
	helloPacket << (int)UDP_SYSTEM_MESSAGE::HELLO;

	std::cout << "[UDP CLIENT] Insert a bunch of random numbers ( /!\ ONLY NUMBERS /!\ ) and press ENTER." << std::endl;

	unsigned int inputNumbers;
	std::cin >> inputNumbers;

	clientSalt = inputNumbers;

	helloPacketId = AddCriticalPacket(helloPacket);

	isRunning = true;
	std::thread mainThread(&UDP::Client::ManageSocket, this);
	std::thread criticalPacketsThread(&UDP::Client::CriticalPacketsManager, this);
	mainThread.detach();
	criticalPacketsThread.detach();
}

#pragma endregion