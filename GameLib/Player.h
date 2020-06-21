#pragma once
#include <SFML\Graphics.hpp>
#include "AssetManager.h"

class Player
{
private:
	sf::Sprite sprite;

public:
	void SetPos(sf::Vector2f pos);
	void SetScale(sf::Vector2f scale);
	void DrawPlayer(sf::RenderWindow *window);
	void ShootBullet();
	void MoveAllBullets();
	sf::Vector2f GetPos();
	Player();	std::vector<sf::Sprite> bullets;
};

