#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <Player.h>
#include "VortexOnline.h"
#include "Protocol.h"
#include <algorithm>
#include "Functions.h"

//ITEMS que contorla el servidor
int dado1, dado2;
//resultat son les 3 cartes "guanyadores"
std::list<carta> baraja, resultat;

std::vector<PlayerInfo> players;
std::map<unsigned int, Player> playersUDP;
std::list<sf::TcpSocket*>::iterator currentPlayer;
int currentPlayer_id;
sf::Packet packet;


//Posar el enum amb les comandes a Playerinfo enlloc de protocol.h?

//TCP Protocols
void TCPTestingProtocol(Network::TCP::Server &server, sf::Packet& packet, int roomIndex, int socketIndex) 
{

}
//void TCPProtocol(Network::TCP::Server &server, sf::Packet& packet, int roomIndex, int socketIndex) {
//	
//	// Switch amb els diferents missatges a enviar o rebre dels clients?
//
//	int intHead;
//	packet >> intHead;
//	Header head = (Header)intHead;
//
//	switch (head)
//	{
//	//Arriba un client nou per jugar
//	case CONNECT:
//		if (roomIndex == -1 && TCP_SERVER.RoomsCount() == 0) { //El jugador esta a la llista d'espera i no hi ha sales creades
//			TCP_SERVER.CreateRoom(TCP_SERVER.GetMaxPlayersRoom());
//			TCP_SERVER.JoinRoom(socketIndex, 0);
//		}
//		else if (roomIndex == -1 && TCP_SERVER.RoomsCount() > 0) { //El jugador esta a la llista d'espera i hi ha sales creades
//			for (int i = 0; i < TCP_SERVER.RoomsCount(); i++) {
//				if (TCP_SERVER.GetRoom(i).GetSocketsCount() < TCP_SERVER.GetRoom(i).MaxClients()) { //Hi ha espai a la sala
//					TCP_SERVER.JoinRoom(socketIndex, i);
//					if (TCP_SERVER.GetRoom(i).GetSocketsCount() == TCP_SERVER.GetRoom(i).MaxClients()) { // L'habitació ja està plena, enviem el START de partida als clients que hi ha dins
//						std::cout << "Room " << i << " will start the game." << std::endl;
//						sf::Packet startPacket;
//						startPacket << 1 << TCP_SERVER.GetRoom(i).GetSocketsCount();
//						/*for (int j = 0; j < TCP_SERVER.GetRoom(i).GetSocketsCount(); j++) {
//							startPacket << j << positionX << positionY << numero de cartes << idCartes;
//						}*/
//						TCP_SERVER.BroadcastSend(i, startPacket);
//					}
//					break;
//				}
//			}
//		}		
//		
//		break;
//		//Enviar als clients que comença la partida
//	case START:
//
//		break;
//
//		//Enviar posicions dels jugadors a tots els clients
//	case UPDATE_POSITIONS:
//
//		break;
//
//		//Enviar al client que li toqui el torn
//	case YOUR_TURN:
//
//		break;
//
//		
//	case Header::MOVE:
//
//		break;
//
//		//
//	case GUESS:
//
//		break;
//
//		//
//	case SUPOSE:
//
//		break;
//
//		//
//	case ACUSE:
//
//		break;
//
//		//El client que fa la acusacio encerta i acaba la partida.
//	case ACUSE_SUCCESS:
//
//		break;
//
//		//El client que fa la acusacio falla i es eliminat
//	case ACUSE_FAIL:
//
//		break;
//
//		//
//	case SHOW_CARDS:
//
//		break;
//
//		//
//	case SHOWING_CARDS:
//
//		break;
//
//	}
//}

//UDP Protocols
//void UDPTestingProtocol(Network::UDP::Server &server, ConnectionData dir, sf::Packet &packet) 
//{
//	//server.
//	//unsigned int id;
//	//if (server.GetConnectionId(dir, id)) {
//	//	server.RemoveCriticalPacket()
//	//}
//
//	int intHead;
//	packet >> intHead;
//	HeaderUDP head = (HeaderUDP)intHead;
//
//	switch (head)
//	{
//	case MOVE:
//		break;
//	case UPDATE_POSITIONSUDP:
//		///Send the new players positions to the others.
//		break;
//	case STARTUDP:
//		///Recivir la ip puerto de cliente existente o nuevo. 
//		break;
//	}
//
//}

//roomindex > 0 esta dins d'una sala.

//Funcions del joc controlades pel servidor

//Crear cartes
void CreateCards()
{
	// 6 Player Cards
	baraja.push_back(carta(CHARACTER, "Amapola", 1));
	baraja.push_back(carta(CHARACTER, "Rubio", 2));
	baraja.push_back(carta(CHARACTER, "Orquidea", 3));
	baraja.push_back(carta(CHARACTER, "Prado", 4));
	baraja.push_back(carta(CHARACTER, "Celeste", 5));
	baraja.push_back(carta(CHARACTER, "Mora", 6));

	// 6 Weapon Cards
	baraja.push_back(carta(WEAPON, "Puï¿½al", 7));
	baraja.push_back(carta(WEAPON, "Cuerda", 8));
	baraja.push_back(carta(WEAPON, "Candelabro", 9));
	baraja.push_back(carta(WEAPON, "Pistola", 10));
	baraja.push_back(carta(WEAPON, "Tuberia de Plomo", 11));
	baraja.push_back(carta(WEAPON, "Herramienta", 12));

	// 9 Rooms
	baraja.push_back(carta(ROOM, "Biblioteca", 13));
	baraja.push_back(carta(ROOM, "Cocina", 14));
	baraja.push_back(carta(ROOM, "Billar", 15));
	baraja.push_back(carta(ROOM, "Baile", 16));
	baraja.push_back(carta(ROOM, "Invernadero", 17));
	baraja.push_back(carta(ROOM, "Comedor", 18));
	baraja.push_back(carta(ROOM, "Vestibulo", 19));
	baraja.push_back(carta(ROOM, "Salon", 20));
	baraja.push_back(carta(ROOM, "Estudio", 21));
}

// Repartir les cartes

void RepartirCartes(int roomIndex)
{
	int random;
	int nPlayers = TCP_SERVER.GetRoom(roomIndex).GetSocketsCount();
	for (int i = 0; baraja.size() != 0; i++) 
	{
		if (i >= nPlayers) i = 0;
		std::list<carta>::iterator it = baraja.begin();
		if (baraja.size() > 0)
		{
			random = rand() % baraja.size();
			std::advance(it, random);
			players[roomIndex + i].myCards.push_back(*it);
			baraja.erase(it);
		}
	}
}

// Enviar Pista
void EnviarPistes()
{
	//<SHOW_CARDS>_<ORIGIN_PID>_<END_PID>_<N_MATCHING_CARDS>_<MATCHING_CARD_IDs...>
}

// Tirar daus
void TirarDaus(int roomIndex, int socketIndex)
{
	bool pista = false;
	dado1 = rand() % 6 + 1;
	dado2 = rand() % 6 + 1;

	if (dado1 == 1 || dado2 == 1) // Si treiem un 1 ensenyem pista
	{
		pista = true;
	}
	
	//enviar al client el resultat de la tirada
	// Falta definir a quin client 
	packet << 2 << dado1 << dado2 << pista;
	TCP_SERVER.Send(roomIndex, socketIndex, packet);
}


// Setup resultado partida
std::list<carta> DefinirResultat(std::list<carta> &baraja)
{
	std::list<carta> c;
	std::list<carta>::iterator it = baraja.begin();

	int r = rand() % 6;
	std::advance(it, r);
	c.push_back(*it);
	baraja.erase(it);

	r = rand() % 5 + 6;
	it = baraja.begin();
	std::advance(it, r);
	c.push_back(*it);
	baraja.erase(it);
	return c;

	
}




//TCP Protocols
void TCPProtocol(Network::TCP::Server &server, sf::Packet& packet, int roomIndex, int socketIndex) {

	// Switch amb els diferents missatges a enviar o rebre dels clients?

	int intHead;
	packet >> intHead;
	Header head = (Header)intHead;

	switch (head)
	{
		//Entra un jugador nou
	case START:

		if (roomIndex == -1 && TCP_SERVER.RoomsCount() == 0) { //El jugador esta a la llista d'espera i no hi ha sales creades
			TCP_SERVER.CreateRoom(TCP_SERVER.GetMaxPlayersRoom());
			players.push_back(PlayerInfo());
			TCP_SERVER.JoinRoom(socketIndex, 0);
		}
		else if (roomIndex == -1 && TCP_SERVER.RoomsCount() > 0) { //El jugador esta a la llista d'espera i hi ha sales creades
			for (int i = 0; i < TCP_SERVER.RoomsCount(); i++) {
				if (TCP_SERVER.GetRoom(i).GetSocketsCount() < TCP_SERVER.GetRoom(i).MaxClients()) { //Hi ha espai a la sala
					players.push_back(PlayerInfo());
					TCP_SERVER.JoinRoom(socketIndex, i);
					if (TCP_SERVER.GetRoom(i).GetSocketsCount() == TCP_SERVER.GetRoom(i).MaxClients()) { // L'habitaciï¿½ ja estï¿½ plena, enviem el START de partida als clients que hi ha dins
						std::cout << "Room " << i << " will start the game." << std::endl;
						CreateCards(); //Creem una baralla de cartes
						RepartirCartes(i); //Barregem les cartes
						for (int j = 0; j < TCP_SERVER.GetRoom(i).GetSocketsCount(); j++) {
							sf::Packet startPacket;
							startPacket << 0 << TCP_SERVER.GetRoom(i).GetSocketsCount();
							for (int k = 0; k < TCP_SERVER.GetRoom(i).GetSocketsCount(); k++) { //ID i posicions de cada jugador
								startPacket << k << 400 << 500;
							}
							startPacket << players[i + j].myCards.size();
							for (std::list<carta>::iterator it = players[i + j].myCards.begin(); it != players[i + j].myCards.end(); ++it) //ID de les cartes que tï¿½ el client
							{
								startPacket << it->id;
							}
							TCP_SERVER.Send(i, j, startPacket);
							std::cout << "Enviem els paquets" << std::endl;
						}	
						TirarDaus(i,0); //Tirem els primers daus de la partida tot assignant qui serï¿½ el jugador que comenï¿½arï¿½
					}
					break;
				}
			}
		}

		break;
		//Enviar als clients que comenï¿½a la partida
	case UPDATE_POSITIONS:

		break;

		//Enviar al client que li toqui el torn
	case YOUR_TURN:

		break;


	case MOVE:

		break;

		//
	case GUESS:

		break;

		//
	case SUPOSE:

		break;

		//
	case ACUSE:

		break;

		//El client que fa la acusacio encerta i acaba la partida.
	case ACUSE_SUCCESS:

		break;

		//El client que fa la acusacio falla i es eliminat
	case ACUSE_FAIL:

		break;

		//
	case SHOW_CARDS:

		break;

		//
	case SHOWING_CARDS:

		break;

	}
}


//UDP Protocols
void UDPProtocol(Network::UDP::Server &server, ConnectionData dir, sf::Packet &packet)
{
	//unsigned int userId;
	//if (server.GetConnectionId(dir, userId)) {
	//	playersUDP
	//}
	int intHead;
	packet >> intHead;
	HeaderUDP head = (HeaderUDP)intHead;

	switch (head)
	{
	case STARTUDP:
		break;
	case UPDATE_POSITIONSUDP:
		break;
	case MOVEUDP:
		break;
	default:
		break;
	}

}

bool SimulationProtocol(Network::UDP::Server &_server, unsigned int _playerId, sf::Packet packet, sf::Packet* _correctionPacket) {

	int command;
	packet >> command;
	UDPGameCommands currentCommand = (UDPGameCommands)command;
	bool result = true;
	
	sf::Vector2f newPos;
	
	switch (currentCommand)
	{
	case MOVE_COMMAND:
		packet >> newPos.x;
		packet >> newPos.y;

		if (newPos.x > 1000 || newPos.x < 0 || newPos.y > 1000 || newPos.y < 0) {
			float x = clamp(newPos.x, 0, 1000);
			float y = clamp(newPos.y, 0, 1000);

			result = false;

			*_correctionPacket << UDPGameCommands::MOVE_COMMAND << _playerId << x << y;

			newPos.x = x;
			newPos.y = y;

		}
		
		playersUDP[_playerId].SetPos(newPos);


		break;
	default:

		break;
	}

	_server.SendBroadcastWithException(*_correctionPacket, _playerId);
	return result;
}

void UDPServerMain() {
	UDP_SERVER.Run(UDPProtocol, SimulationProtocol, 50000);
	
	//while(!startGame)
}

int main()
{
	UDPServerMain();
	//PlayerInfo playerInfo;
	//TCP_SERVER.Run(TCPProtocol, 50000, 4, 3);
	//
	//bool startGame = false;
	//int currentPlayersJoined;
	////Partida



	////TCP_SERVER.CreateRoom(3);
	//while (!startGame)
	//{
	//	//Esperant jugadors a la partida

	//};
	return 0;
}
