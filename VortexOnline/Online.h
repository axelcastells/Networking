#pragma once
#include <thread>
#include <iostream>
#include <mutex>
#include <vector>

#include "ServerRoom.h"
#include "Direction.h"
#include "UniqueIdGenerator.h"
#include "Proxy.h"

enum UDP_SYSTEM_MESSAGE {PING, HELLO, CHALLENGE_ID, CHALLENGE_QUESTION, CHALLENGE_ANSWER};
//#define UDP_PING_ID 109619403579
//#define UDP_HELLO_MESSAGE_ID 0
//#define UDP_CHALLENGE_ID 90620269561985632
//#define UDP_CHALLENGE_QUESTION 16401976109743
//#define UDP_CHALLENGE_ANSWER 937937698

namespace Network {

	namespace UDP {

		class Server {
		public:
			static Server &Instance();
			void Run(void(*funcProtocol)(Server &server, ConnectionData dir, sf::Packet& packet), short _port, unsigned int criticTimer = 1000, unsigned int pingTime = 1000, unsigned int _disconnectPingCycles = 3, bool debug = false);
			void Stop();
			void Shutdown();
			bool GetConnectionData(unsigned int _userId, ConnectionData** dir);
			bool GetConnectionId(const ConnectionData& proxy, unsigned int & id);
			
			void RemoveCriticalPacket(unsigned int _criticalPacketId);

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

			unsigned int serverSalt;
			sf::Mutex mutex;

			short port;
			sf::UdpSocket socket;


			void RemoveNonMemberCriticalPacket(unsigned int _nonMemberCriticalPacketId);
			unsigned int AddCriticalPacket(ConnectionData dir, sf::Packet _pack);
			void SendCriticalPackets();
			std::map<unsigned int, ConnectionData*> connectionsById;
			//std::map<ConnectionData&, unsigned int> connectionsIds;

			std::map<unsigned int, Proxy> criticalPackets;
			std::map<unsigned int, std::pair<ConnectionData, sf::Packet>> nonMemberCriticalPackets;

			void Debug();

			void ManageSocketsThread();
			void Ping();
			void ManageDisconnections();
			void CriticalPacketsManager();

			void(*FunctionProtocol)(Server &_server, ConnectionData dir, sf::Packet& packet);
		};

		class Client {
		public:
			static Client &Instance();
			void Run(void(*funcProtocol)(Client &client, sf::Packet &_pack), sf::IpAddress _ip, unsigned short _serverPort, unsigned int _criticalPacketMillis = 1000);

			// Add Critical Packet
			unsigned int AddCriticalPacket(sf::Packet _pack);
			void RemoveCriticalPacket(unsigned int _criticalPacketId);


			void Send(sf::Packet _packet);
		private:
			Client();
			bool isRunning;
			void ManageSocket();
			void Pong();

			unsigned int saltChecksum;
			unsigned int clientSalt;
			unsigned int criticalPacketMillis;

			int helloPacketId;

			void SendCriticalPackets();
			void CriticalPacketsManager();

			sf::UdpSocket socketToBootstrapServer;
			sf::IpAddress serverIp;

			std::map<unsigned int, sf::Packet> criticalPackets;

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
			void BroadcastSend(sf::Packet pack);

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