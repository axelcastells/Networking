#pragma once
#include <SFML/Network.hpp>
struct ConnectionData {
	ConnectionData() { pingTime = 0; }
	ConnectionData(sf::IpAddress _ip, short _port) :ip(_ip), port(_port) {}
	ConnectionData(const ConnectionData& data) {
		ip = data.ip;
		port = data.port;
		pingTime = data.pingTime;
		salt = 0;
	}
	sf::IpAddress ip;
	unsigned short port;
	unsigned int pingTime;
	unsigned int salt;


	ConnectionData& operator *() {
		return *this;
	}

	void operator delete(void*p) {
		//free(p);
	}

	void operator++() {
		//++port;
	}

	bool operator <(const ConnectionData &dir) const {
		if (ip.toInteger() + port < dir.ip.toInteger() + dir.port) {
			return true;
		}
		else return false;
	}
	bool operator ==(ConnectionData dir) {
		if (ip == dir.ip && port == dir.port) return true;
		else return false;
	}
	const void operator =(ConnectionData dir) {
		ip = dir.ip;
		port = dir.port;
		pingTime = dir.pingTime;
	}
};