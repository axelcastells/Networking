#pragma once
#include <thread>
#include <iostream>
#include <mutex>
#include <vector>

#include "ServerRoom.h"
#include "Direction.h"
#include "UniqueIdGenerator.h"
#include "Proxy.h"

namespace Network {

	namespace UDP {

		class Server {
		public:
			static Server &Instance();
			void Run(void(*funcProtocol)(Server &server, ConnectionData dir, sf::Packet& packet), sf::Packet _pingPacket, short _port, unsigned int criticTimer = 1000, unsigned int pingTime = 1000, unsigned int _disconnectPingCycles = 3, bool debug = false);
			void Stop();
			void Shutdown();
			bool GetConnectionData(unsigned int _userId, ConnectionData& dir);
			bool GetConnectionId(const ConnectionData& proxy, unsigned int & id);
			unsigned int AddCriticalPacket(unsigned int _playerUid, sf::Packet _pack);

			void AddConnection(unsigned int newUid, ConnectionData dir);

			void PongReceived(unsigned int _userId);
			void Send(sf::Packet _pack, ConnectionData dir);
			void SendBroadcast(sf::Packet _pack, unsigned int _pingTime = 0);
		private:
			Server();
			bool isRunning;
			unsigned int criticalPacketMillis;
			unsigned int pingerMillis;
			unsigned int disconnectPingCycles;

			short port;
			sf::UdpSocket socket;
			sf::Packet pingPacket;

			void ResendCriticalPackets();
			std::map<unsigned int, ConnectionData*> connectionsById;

			std::map<unsigned int, Proxy> criticalPackets;

			void Debug();

			void ManageSocketsThread();
			void Ping();
			void ManageDisconnections();
			void CriticPacketManager();

			void(*FunctionProtocol)(Server &_server, ConnectionData dir, sf::Packet& packet);
		};

		class Client {
		public:
			static Client &Instance();
			void Init(void(*funcProtocol)(Client &client, sf::Packet &_pack), sf::IpAddress _ip, unsigned short _serverPort, unsigned short _localPort);
			void Run(bool _debug = false);

			void Send(sf::Packet _packet);
		private:
			Client();
			bool isRunning;
			void ManageSocket();
			void Pong();
			sf::UdpSocket socketToBootstrapServer;
			sf::IpAddress serverIp;
			unsigned int serverPort;
			void(*FunctionProtocol)(Client &_client, sf::Packet& packet);
		};
	}

	namespace TCP {

		class Peer {
		public:
			static Peer& Instance();
			void Run(void(*funcProtocol)(Peer &_peer, sf::Packet& packet), std::string _ip, short _port, bool debug = false);

			void Send(int peerIndex, sf::Packet pack);


		private:
			Peer();
			void(*FunctionProtocol)(Peer &_peer, sf::Packet& packet);

			void ManagePeers();
			void Debugger();

			sf::SocketSelector socketSelector;
			sf::TcpListener listener;

			int matchedPeersCount;
			std::string ip;
			std::vector<std::pair<std::string, unsigned short>> directions;
			unsigned short connectPort;
			unsigned short localPort;
			sf::TcpSocket socketToBootstrapServer;
			std::vector<sf::TcpSocket*> sockets;
		};

		class BootstrapServer {
		public:
			static BootstrapServer& Instance();
			void Run(int _maxUsers, short _port);
		private:
			BootstrapServer();
			void ManageSockets();
			sf::TcpListener listener;
			sf::SocketSelector socketSelector;
			std::vector<ConnectionData> directions;
			short connectPort;
			int maxUsers;
		};

		class Client {
		public:
			static Client& Instance();
			void Run(void(*funcProtocol)(Client &_client, sf::Packet& packet), std::string _ip, short _port);

			void Send(sf::Packet& packet);

		private:
			Client();
			void ManageSocket();

			void(*FunctionProtocol)(Client &_client, sf::Packet& packet);

			std::string ip;
			short connectPort;
			sf::TcpSocket socketToBootstrapServer;

		};

		class Server {
		public:
			static Server &Instance();

			void Run(void(*funcProtocol)(Server &_server, sf::Packet& packet, int roomIndex, int socketIndex), short _port, int _maxRooms = 1, bool debug = false);

			// Returns new room index
			int CreateRoom(int maxClients);

			//Closes a room by index
			void CloseRoom(int index);
			void JoinRoom(int unassignedSocketIndex, int toRoomIndex);
			void ExitRoom(int roomIndex, int socketIndex);
			int RoomsCount();
			ServerRoom &GetRoom(int index);

			void Send(int sock, sf::Packet& packet);
			void Send(int room, int sock, sf::Packet& packet);
			void BroadcastSend(sf::Packet& packet);
			void BroadcastSend(int _roomIndex, sf::Packet& packet);

		private:
			Server();
			void Debugger();
			void ManageSockets();
			sf::TcpSocket &LastSocket();
			int LastSocketIndex();

			int maxServerRooms;
			short connectPort;

			void(*FunctionProtocol)(Server &_server, sf::Packet& packet, int roomIndex, int socketIndex);

			sf::TcpListener listener;
			sf::SocketSelector socketSelector;

			std::vector<sf::TcpSocket*> unassignedSockets;
			std::vector<ServerRoom> serverRooms;
		};
	}
}