#pragma once
#include <list>
#include <SFML\Graphics.hpp>

enum Cardtype { CHARACTER , WEAPON, ROOM};

enum State { YOURTURN, WAIT};

class PlayerInfo
{
	enum class Color;
	std::string name;
	sf::Vector2i position;
	Color color;
	int lives;

public:

	enum class Color {
		RED,
		GREEN,
		BLUE,
		YELLOW,
		PURPLE,
		ORANGE
	};

	PlayerInfo();
	PlayerInfo(std::string);
	PlayerInfo(std::string, unsigned short);
	~PlayerInfo();

	std::string GetName();
	void SetName(std::string _name);
	Color GetColor();
	void SetColor(Color _color);
	unsigned short GetIdColor();
	Color IdColorToColor(unsigned short);

	std::list<carta> myCards;
};

class carta
{
public:
	Cardtype tipo;
	std::string Cardname;
	int id;

	carta ( Cardtype _tipo, std::string _cardname, int _id)
	{
		tipo = _tipo;
		Cardname = _cardname;
		id = _id;
	}

private:
};