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

PlayerInfo::PlayerInfo(std::string _name, unsigned short _color) : name(_name), position(sf::Vector2i(0, 0)), lives(3), color(GetColor()) {}

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

sf::Vector2i PlayerInfo::GetPosition()
{
	return position;
}

void PlayerInfo::SetPosition(sf::Vector2i _Position)
{
	position = _Position;
}
void PlayerInfo::SetPosition(int _x, int _y)
{
	position.x = _x;
	position.y = _y;
}