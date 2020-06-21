#include "PlayerCluedo.h"
#include <random>

PlayerCLUEDO::PlayerCLUEDO()
{
	sprite = sf::Sprite(AssetManager::GetTexture("sprite1.png"));
}

void PlayerCLUEDO::SetPos(sf::Vector2f position)
{
	sprite.setPosition(position);
}

void PlayerCLUEDO::SetScale(sf::Vector2f scale)
{
	sprite.setScale(scale);
}

void PlayerCLUEDO::DrawPlayer(sf::RenderWindow * window)
{
	window->draw(sprite);
}

sf::Vector2f PlayerCLUEDO::GetPos()
{
	return sprite.getPosition();
}
