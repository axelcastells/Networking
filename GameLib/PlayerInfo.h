#pragma once
#include <SFML\Graphics.hpp>

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
};