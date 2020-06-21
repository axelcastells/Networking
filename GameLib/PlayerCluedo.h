#pragma once
#include <SFML\Graphics.hpp>
#include "AssetManager.h"

class PlayerCLUEDO
{
private:
	sf::Sprite sprite;
public:
	void SetPos(sf::Vector2f pos);
	void SetScale(sf::Vector2f scale);
	void DrawPlayer(sf::RenderWindow *window);
	sf::Vector2f GetPos();
	PlayerCLUEDO();
};

