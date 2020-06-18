#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>

class AssetManager 
{
public:
	AssetManager();
	static sf::Texture& GetTexture(std::string const&filename);
private:
	std::map<std::string, sf::Texture> textures;
	//singletone para acceder desde cualquier lugar al objeto de la clase.
	static AssetManager* sInstance;
};