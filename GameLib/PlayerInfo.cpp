#include "PlayerInfo.h"
#include <random>


PlayerInfo::PlayerInfo()
{
	position.x = std::rand() % 10;
	position.y = std::rand() % 10;
	lives = 3;
}

PlayerInfo::~PlayerInfo()
{
}

PlayerInfo::PlayerInfo(std::string _name) :
	name(_name), position(sf::Vector2i(0, 0)), lives(3), color(Color::BLUE) {}

PlayerInfo::PlayerInfo(std::string _name, unsigned short _color) : name(_name), position(sf::Vector2i(0, 0)), lives(3), color(IdColorToColor(_color)) {}

PlayerInfo::~PlayerInfo()
{
}

std::string PlayerInfo::GetName()
{
	return name;
}

void PlayerInfo::SetName(std::string _name) {
	name = _name;
}

PlayerInfo::Color PlayerInfo::GetColor()
{
	return color;
}

void PlayerInfo::SetColor(PlayerInfo::Color _color) {
	color = _color;
}

unsigned short PlayerInfo::GetIdColor()
{
	return static_cast<unsigned short>(color);
}

PlayerInfo::Color PlayerInfo::IdColorToColor(unsigned short _idColor) {
	Color _color = Color::RED;

	if (_idColor == 0) _color = Color::RED;
	else if (_idColor == 1) _color = Color::GREEN;
	else if (_idColor == 2) _color = Color::BLUE;
	else if (_idColor == 3) _color = Color::YELLOW;
	else if (_idColor == 4) _color = Color::PURPLE;
	else if (_idColor == 5) _color = Color::ORANGE;

	return _color;
}