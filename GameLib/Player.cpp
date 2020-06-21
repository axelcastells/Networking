#include "Player.h"
#include <random>

Player::Player()
{
	std::string folder = "images/";
	sprite = sf::Sprite(AssetManager::GetTexture(folder + "player.png"));
}

void Player::SetPos(sf::Vector2f position)
{
	sprite.setPosition(position);
}

void Player::SetScale(sf::Vector2f scale)
{
	sprite.setScale(scale);
}

void Player::DrawPlayer(sf::RenderWindow * window)
{
	window->draw(sprite);
}

void Player::ShootBullet()
{
	std::string folder = "images/";
	sf::Sprite bullet = sf::Sprite(AssetManager::GetTexture(folder + "player.png"));
	bullet.setColor(sf::Color(255, 255, 0));
	bullet.setPosition(sf::Vector2f(sprite.getPosition().x + 2, sprite.getPosition().y));
	bullets.push_back(bullet);
}

void Player::MoveAllBullets()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i].setPosition(
			sf::Vector2f(bullets[i].getPosition().x + 3.0f, bullets[i].getPosition().y)
		);
	}
}

sf::Vector2f Player::GetPos()
{
	return sprite.getPosition();
}
