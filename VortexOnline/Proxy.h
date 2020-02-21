#pragma once
#include <SFML/Network.hpp>
#include "Direction.h"
struct Proxy {
	Proxy() {}
	Proxy(sf::Packet _pack, unsigned int _playerUid) :packet(_pack), playerUid(_playerUid) {

	}
	sf::Packet packet;
	unsigned int playerUid;
};