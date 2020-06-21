#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>

class AssetManager 
{
public:
	AssetManager();
	static sf::Texture& GetTexture(std::string const&filename);
	static std::vector<std::string> GetNames();
	static void DrawAllSprites(sf::RenderWindow *window);	
	static void PushSprite(sf::Sprite sprite);
private:
	//singletone para acceder desde cualquier lugar al objeto de la clase.
	std::map<std::string, sf::Texture> textures;
	std::vector<sf::Sprite> sprites;
	static AssetManager* sInstance;
};