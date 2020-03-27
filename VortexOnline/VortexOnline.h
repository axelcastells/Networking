#pragma once
#include "Online.h"

#define TCP_SERVER Network::TCP::Server::Instance()
#define TCP_CLIENT Network::TCP::Client::Instance()
#define TCP_PEER Network::TCP::Peer::Instance()
#define TCP_BTSSERVER Network::TCP::BootstrapServer::Instance()

#define UDP_SERVER Network::UDP::Server::Instance()
#define UDP_CLIENT Network::UDP::Client::Instance()