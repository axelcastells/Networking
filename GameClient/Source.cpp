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
#include <AssetManager.h>


//Client local setup
PlayerInfo playerInfo;
sf::TcpSocket serverSocket;
sf::Packet loginPacket;
bool startGame = false;
int maxPlayers;
int currentPlayersJoined;
std::vector<PlayerInfo> players;
std::vector<std::string> packets;

int positionX, positionY;

std::vector<carta> hand;
int size = 0;
int receivedCard = 0;
int dado1, dado2;
int dados = dado1 + dado2;
bool pista = false;
int pistaType = 0;
std::string escollirPista;
std::string suposePlayer;
std::string Murderer, Weapon, Room;

State estadoPlayer = WAIT;
sf::Event event;
bool end_game = false;


Graphics g;


//TCP PROTOCOLS
void TCPTestingProtocol(Network::TCP::Client &client, sf::Packet &packet)
{

}
void TCPProtocol(Network::TCP::Client &client, sf::Packet &packet) {

	//Switch amb els diferents tipus de packets rebuts

	int intHead;
	packet >> intHead;
	Header head = (Header)intHead;

	//Només missatges rebuts, respostes al main.
	switch (head)
	{
	case START:
		startGame = true;
		/*
		// Missatge rebut per començar la partida
		//DESMONTAR PACKET -> <START>_<NPLAYERS>_<PLAYERID>_<PLAYERPOSITIONS>
							//_<GIVENCARDS>_<IDSCARTES>

		//FET
		
		packet >> currentPlayersJoined;

		// Recorrer el vector per assignar les IDs a cadascun
		for (int i = 0; i < currentPlayersJoined; i++)
		{
			int _color;
			PlayerInfo player;
			packet >> _color; // PLAYERID

			// Asignar la ID al jugador.
			players[i].SetColor((PlayerInfo::Color)_color);

			//Asignar la posició inicial al jugador.
			packet >> positionX >> positionY; //PLAYERPOSITIONS

			player.SetPosition(positionX, positionY);
			std::cout << "El jugador " << (int)players[i].GetColor() << " comença en la posició X: " << players[i].GetPosition().x << " Y: " << players[i].GetPosition().y << std::endl;
			players.push_back(player);
		}

		// Assignar les posicions amb el vector com hem fet amb el color
		//packet >> playerInfo.position;
		
		// Cartes rebudes
		packet >> size; //GIVENCARDS quantes cartes rebo

		std::cout << "Has rebut %d " << size << " Cartes" << std::endl;
		for (int i = 0; i < size; i++)
		{
			packet >> receivedCard; //IDCARTES
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
		*/
		packet.clear();
		break;

	case UPDATE_POSITIONS:

		//DESMONTAR PACKET -> <UPDATE_POSITIONS>_<PLAYER_ID>_<NEW_POS>

		//Missatge rebut per actualitzar la posició dels jugadors
		for (int i = 0; i < currentPlayersJoined; i++)
		{
			
			PlayerInfo player;
			//packet >> players[i].SetColor;
			packet >> positionX >> positionY;

			player.SetPosition(positionX,positionY);
			std::cout << "El jugador " << (int)players[i].GetColor() << " esta en la posició X: " << players[i].GetPosition().x << " Y: " << players[i].GetPosition().y << std::endl;
			players.push_back(player);
		}
		packet.clear();
		break;

	case YOUR_TURN:

		// <YOUR_TURN>_<DICE_1>_<DICE_2>_<CLUE_CARDID>

		//Missatge rebut quan et toca el teu torn
		estadoPlayer = YOURTURN;

		packet >> dado1 >> dado2;
		std::cout << "En la teva tirada has tret un " << dado1 << " y un " << dado2 << std::endl;
		

		packet >> pista; //Bool de si ens toca pista

		if (!pista)
		{
			std::cout << "Sense pista, no has tret un 1." << std::endl;
		}
		else 
		{
			std::cout << "Pots escollir pista." << std::endl;

			std::cout << "Quin tipus de pista vols rebre? 1- Personatge | 2- Arma | 3- Habitació" << std::endl;
			std::cin >> pistaType;
			
			switch (pistaType)
			{
			case 0: // PERSONATGE
				std::cout << "Tipo: Personatge, Escull entre els següents personatges: Amapola | Rubio | Orquidea | Prado | Celeste | Mora." << std::endl;
				while (escollirPista != "Amapola" && escollirPista != "Rubio" && escollirPista != "Orquidea" && escollirPista != "Prado" && escollirPista != "Celeste" && escollirPista != "Mora")
				{
					std::cout << "Introdueix el nom del personatge que vols obtenir la pista." << std::endl;
					std::cin >> escollirPista;
				}
				break;

			case 1: // Arma
				std::cout << "Tipo: Arma, Escull entre les següents armes: Puñal | Cuerda | Candelabro | Pistola | Tuberia de Plomo | Herramienta." << std::endl;
				while (escollirPista != "Puñal" && escollirPista != "Cuerda" && escollirPista != "Candelabro" && escollirPista != "Pistola" && escollirPista != "Herramienta" && escollirPista != "Tuberia de Plomo" && escollirPista != "Tuberia")
				{
					std::cout << "Introdueix el nom de l'arma que vols obtenir la pista." << std::endl;
					std::cin >> escollirPista;
				}
				break;

			case 2: // Habitació
				std::cout << "Tipo: Habitacio, Escull entre les següents habitacions: Biblioteca | Cocina | Billar | Baile | Invernadero | Comedor | Vestibulo | Salon | Estudio" << std::endl;
				while (escollirPista != "Biblioteca" && escollirPista != "Cocina" && escollirPista != "Billar" && escollirPista != "Baile" && escollirPista != "Invernadero" && escollirPista != "Comedor" && escollirPista != "Vestibulo" && escollirPista != "Salon" && escollirPista != "Estudio")
				{
					std::cout << "Introdueix el nom de l'habitacio que vols obtenir la pista." << std::endl;
					std::cin >> escollirPista;
				}
				break;

			default:
				break;
			}
		}
		
		//Falta el moviment, montar el packet amb el tipus de pista que vol i enviarlo

		packet.clear();
		break;

	case MOVE:

		packet.clear();
		break;

		//Rebre el supose d'un altre client
	case SUPOSE:

		
		packet >> suposePlayer;
		std::cout << suposePlayer << std::endl;
		packet.clear();
		break;

		//Rebre el acuse d'un altre client
	case ACUSE:

		// <PLAYER_ID>_<ACUSE>_<MURDERER>_<WEAPON>_<ROOM>

		packet >> Murderer >> Weapon >> Room;
		std::cout << "El jugador: " << "PlayerID" << " fa la següent acusació " << "l'assasí es: " << Murderer << " amb l'arma " << Weapon << "a la sala " << Room << std::endl;
		packet.clear();
		break;

		//Rebre la notificació de qui guanya la partida i acabar
	case ACUSE_SUCCESS:

		packet.clear();
		break;

		//Rebre la notificacio de qui falla i queda eliminat
	case ACUSE_FAIL:

		packet.clear();
		break;

		//Llegir quin client esta ensenyant cartes a un altre
	case SHOWING_CARDS:

		// <PLAYER_ID_SENDER>_<PLAYER_ID_RECEIVER>

		//Comprobar la ID i donar la info
		int playerIDSender, playerIDReceiver;
		/*PlayerInfo _playerS, _playerR;

		for (int i = 0; i < currentPlayersJoined; i++)
		{
			
			PlayerInfo _player;
			if (_player.GetColor() == _playerS.GetColor)
			{

			}
		}
		_playerS = players[i];*/
		packet >> playerIDSender >> playerIDReceiver;

		std::cout << "El jugador : " << playerIDSender << " esta ensenyant una carta al jugador: " << playerIDReceiver << std::endl;

		packet.clear();
		break;

	}
}

//UDP PROTOCOLS
void UDPTestingProtocol(Network::UDP::Client &client, sf::Packet &packet) {
	sf::Packet commandPacket;
	commandPacket << 999;
	//UDP_CLIENT.AddCommand(commandPacket);
}

 
//int Run() {
//	//TCP_CLIENT.Run(TCPProtocol, "localhost", 50000);
//	UDP_CLIENT.Run(UDPTestingProtocol, "localhost", 50000);
//	sf::Packet connectPacket;
//	connectPacket << 111;
//	//TCP_CLIENT.Send(connectPacket);
//	return 0;
//}

std::string folder = "images/";

void AddSprite(std::string name, sf::Vector2f pos, sf::Vector2f scale)
{

	sf::Sprite sprite = sf::Sprite(AssetManager::GetTexture(folder + name));
	sprite.setPosition(pos);
	sprite.setScale(scale);
	AssetManager::PushSprite(sprite);
}

void Cluedo() 
{
	sf::RenderWindow window(sf::VideoMode(800, 1000), "Game");
	window.setFramerateLimit(60);

	AssetManager manager;

	//Taulell(manager);

	sf::Vector2f taulell[9];

	taulell[0] = sf::Vector2f(0, 0);
	taulell[1] = sf::Vector2f(260, 0);
	taulell[2] = sf::Vector2f(530, 0);
	taulell[3] = sf::Vector2f(0, 260);
	taulell[4] = sf::Vector2f(0, 0);

	///SPRITES TAULLEL
	sf::Vector2f scaleRooms = sf::Vector2f(0.65f, 0.65f);
	///
	AddSprite("SALON.png", sf::Vector2f(0, 0), scaleRooms);
	AddSprite("CONSERVATORIO.png", sf::Vector2f(260, 0), scaleRooms);
	AddSprite("SALADEBAILE.png", sf::Vector2f(530, 0), scaleRooms);
	///
	AddSprite("COMEDOR.png", sf::Vector2f(0, 260), scaleRooms);
	AddSprite("COCINA.png", sf::Vector2f(260, 260), scaleRooms);
	AddSprite("BIBLIOTECA.png", sf::Vector2f(530, 260), scaleRooms);
	///
	AddSprite("SALABILLAR.png", sf::Vector2f(0, 520), scaleRooms);
	AddSprite("ESTUDIO.png", sf::Vector2f(260, 520), scaleRooms);
	AddSprite("SALA.png", sf::Vector2f(530, 520), scaleRooms);
	///

	std::vector<std::string> names = AssetManager::GetNames();
	for (int i = 0; i < names.size(); i++)
	{
		std::cout << names[i] << std::endl;
	}
	//Init(); //Despues de la creacion de la ventana y propiedades. 

	///PLAYER COMENTADO
	//PlayerCLUEDO *player = new PlayerCLUEDO();
	//player->SetScale(sf::Vector2f(0.5f, 0.5f));
	//player->SetPos(sf::Vector2f(10, 50));


	sf::Clock clock;
	sf::Time elapsedTime;

	while (window.isOpen())
	{
		//Time. 
		sf::Time deltaTime = clock.restart();
		elapsedTime += deltaTime;

		//Make the loop game. 
		//1. Input.  
		sf::Event event; // variable ref. 
		while (window.pollEvent(event)) 
		{

			switch (event.type) {
			case sf::Event::EventType::Closed:
				window.close();
				break;

			case sf::Event::KeyPressed:
				/*if (event.key.code == sf::Keyboard::Up)
				{
					sf::Vector2f playerPos = player->GetPos();
					player->SetPos(
						sf::Vector2f(playerPos.x, playerPos.y - 260)
					);
				}

				if (event.key.code == sf::Keyboard::Down)
				{
					sf::Vector2f playerPos = player->GetPos();
					player->SetPos(
						sf::Vector2f(playerPos.x, playerPos.y + 260)
					);
				}

				if (event.key.code == sf::Keyboard::Left)
				{
					sf::Vector2f playerPos = player->GetPos();
					player->SetPos(
						sf::Vector2f(playerPos.x - 260, playerPos.y)
					);
				}

				if (event.key.code == sf::Keyboard::Right)
				{
					sf::Vector2f playerPos = player->GetPos();
					player->SetPos(
						sf::Vector2f(playerPos.x + 260, playerPos.y)
					);
				}*/

				break;
			}
		}
		//2. Update.
		///Constraints 
		//sf::Vector2f playerPos = player->GetPos();
		//if (playerPos.y < 0)
		//	player->SetPos(sf::Vector2f(playerPos.x, 0.0f));
		//if (playerPos.y > 520)
		//	player->SetPos(sf::Vector2f(playerPos.x, 520));
		//if (playerPos.x < 0)
		//	player->SetPos(sf::Vector2f(0.0f, playerPos.y));
		//if (playerPos.x > 530)
		//	player->SetPos(sf::Vector2f(530, playerPos.y));

		//3. Clear. 
		window.clear(sf::Color::Black);
		//4. Draw or Render: Dibjar objetos creados. 
		AssetManager::DrawAllSprites(&window);
		//player->DrawPlayer(&window);
		window.display(); // Ultimo metodo a llamar. 
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
	
	//TEST AQUI
	/*hand.push_back(carta(CHARACTER, "Mora", 6));
	std::cout << "Carta 1: " << hand[0].Cardname << std::endl;*/
	

	//Connect a Servidor
	UDP_CLIENT.Run(UDPTestingProtocol, "localhost", 50000);

	sf::Packet commandPacket;
	commandPacket << 75;
	//std::cout << commandPacket << std::endl;
	//while (true) {

	//UDP_CLIENT.AddCommand(commandPacket);
	//}

	///PARTIDA
	Cluedo();


	//Partida
	//while (!startGame)
	//{
	//	//Esperant jugadors a la partida

	//};
	
	//g.DrawDungeon();


	return 0;
}