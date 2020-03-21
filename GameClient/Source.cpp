#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include "Graphics.h"
#include "VortexOnline.h"
#include "Protocol.h"

//Client local setup
PlayerInfo playerInfo;
sf::TcpSocket serverSocket;
sf::Packet loginPacket;
bool startGame = false;
int maxPlayers;
int currentPlayersJoined;
std::vector<PlayerInfo> players;
std::vector<std::string> packets;

std::vector<carta> hand;
int size = 0;
int receivedCard = 0;
int dado1, dado2;

State estadoPlayer = WAIT;
sf::Event event;
bool end_game = false;


Graphics g;



void Protocol(Network::TCP::Client &client, sf::Packet &packet) {

	//Switch amb els diferents tipus de packets rebuts

	int intHead;
	packet >> intHead;
	Header head = (Header)intHead;

	//Només missatges rebuts, respostes al main.
	switch (head)
	{
	case START:

		// Missatge rebut per començar la partida
		//DESMONTAR PACKET -> <START>_<NPLAYERS>_<PLAYERID>_<PLAYERPOSITIONS>
							//_<COLOR>_<GIVENCARDS>_<IDSCARTES>

		
		packet >> currentPlayersJoined;

		// Recorrer el vector per assignar les IDs a cadascun
		//packet >> players.color;

		// Assignar les posicions amb el vector com hem fet amb el color
		//packet >> playerInfo.position;
		
		// Cartes rebudes
		packet >> size; //quantes cartes rebo

		std::cout << "Has rebut %d " << size << " Cartes" << std::endl;
		for (int i = 0; i < size; i++)
		{
			packet >> receivedCard;
			switch (receivedCard)
			{
				//CHARACTERS
			case 1:
				hand.push_back(carta(CHARACTER, "Amapola", 1));
				std::cout << "Has rebut la carta: " << "Amapola" << std::endl;
				break;
			case 2:
				hand.push_back(carta(CHARACTER, "Rubio", 1));
				std::cout << "Has rebut la carta: " << "Rubio" << std::endl;
				break;
			case 3:
				hand.push_back(carta(CHARACTER, "Orquidea", 3));
				std::cout << "Has rebut la carta: " << "Orquidea" << std::endl;
				break;
			case 4:
				hand.push_back(carta(CHARACTER, "Prado", 4));
				std::cout << "Has rebut la carta: " << "Prado" << std::endl;
				break;
			case 5:
				hand.push_back(carta(CHARACTER, "Celeste", 5));
				std::cout << "Has rebut la carta: " << "Celeste" << std::endl;
				break;
			case 6:
				hand.push_back(carta(CHARACTER, "Mora", 6));
				std::cout << "Has rebut la carta: " << "Mora" << std::endl;
				break;

				//WEAPONS
			case 7:
				hand.push_back(carta(WEAPON, "Puñal", 7));
				std::cout << "Has rebut la carta: " << "Puñal" << std::endl;
				break;
			case 8:
				hand.push_back(carta(WEAPON, "Cuerda", 8));
				std::cout << "Has rebut la carta: " << "Cuerda" << std::endl;
				break;
			case 9:
				hand.push_back(carta(WEAPON, "Candelabro", 9));
				std::cout << "Has rebut la carta: " << "Candelabro" << std::endl;
				break;
			case 10:
				hand.push_back(carta(WEAPON, "Pistola", 10));
				std::cout << "Has rebut la carta: " << "Pistola" << std::endl;
				break;
			case 11:
				hand.push_back(carta(WEAPON, "Tuberia de Plomo", 11));
				std::cout << "Has rebut la carta: " << "Tuberia de Plomo" << std::endl;
				break;
			case 12:
				hand.push_back(carta(WEAPON, "Herramienta", 12));
				std::cout << "Has rebut la carta: " << "Herramienta" << std::endl;
				break;

				//ROOMS
			case 13:
				hand.push_back(carta(ROOM, "Biblioteca", 13));
				std::cout << "Has rebut la carta: " << "Biblioteca" << std::endl;
				break;
			case 14:
				hand.push_back(carta(ROOM, "Cocina", 14));
				std::cout << "Has rebut la carta: " << "Estudio" << std::endl;
				break;
			case 15:
				hand.push_back(carta(ROOM, "Billar", 15));
				std::cout << "Has rebut la carta: " << "Billar" << std::endl;
				break;
			case 16:
				hand.push_back(carta(ROOM, "Baile", 16));
				std::cout << "Has rebut la carta: " << "Baile" << std::endl;
				break;
			case 17:
				hand.push_back(carta(ROOM, "Invernadero", 17));
				std::cout << "Has rebut la carta: " << "Invernadero" << std::endl;
				break;
			case 18:
				hand.push_back(carta(ROOM, "Comedor", 18));
				std::cout << "Has rebut la carta: " << "Comedor" << std::endl;
				break;
			case 19:
				hand.push_back(carta(ROOM, "Vestibulo", 19));
				std::cout << "Has rebut la carta: " << "Vestibulo" << std::endl;
				break;
			case 20:
				hand.push_back(carta(ROOM, "Salon", 20));
				std::cout << "Has rebut la carta: " << "Salon" << std::endl;
				break;
			case 21:
				hand.push_back(carta(ROOM, "Estudio", 21));
				std::cout << "Has rebut la carta: " << "Estudio" << std::endl;
				break;
			default:break;
			}

		}

		break;

	case UPDATE_POSITIONS:

		//Missatge rebut per actualitzar la posició dels jugadors


		break;

	case YOUR_TURN:

		//Missatge rebut quan et toca el teu torn
		estadoPlayer = YOURTURN;

		break;

	case MOVE:


		break;

		//Permet fer una demanda al servidor quan arribi a una sala
	case GUESS:

		break;

		//Rebre el supose d'un altre client
	case SUPOSE:

		break;

		//Rebre el acuse d'un altre client
	case ACUSE:

		break;

		//Rebre la notificació de qui guanya la partida i acabar
	case ACUSE_SUCCESS:

		break;

		//Rebre la notificacio de qui falla i queda eliminat
	case ACUSE_FAIL:

		break;

		//El client ensenya la carta escollida al client pertinent a traves del servidor.
	case SHOW_CARDS:

		break;

		//Llegir quin client esta ensenyant cartes a un altre
	case SHOWING_CARDS:

		break;

	}
}

int main()
{

	// INIT PLAYER
	std::string nickname;
	std::cout << "Enter your nickname: " << std::endl;
	std::cin >> nickname;
	//loginPacket << "JOINED " << nickname;
	playerInfo.SetName(nickname);

	//Connect a Servidor
	TCP_CLIENT.Run(Protocol, "localhost", 50001);

	//Partida
	while (!startGame)
	{
		//Esperant jugadors a la partida


	};

	g.DrawDungeon();


	return 0;
}