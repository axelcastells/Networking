
#include "AssetManager.h"
#include <assert.h>

AssetManager* AssetManager::sInstance = nullptr; //Definir el singletone. 

AssetManager::AssetManager() {
	assert(sInstance == nullptr); //Si esto es false, printea un mensaje. 
	sInstance = this;
}

sf::Texture& AssetManager::GetTexture(std::string const&filename) {

	auto& texMap = sInstance->textures; //Copia real del map de la clase. 

	auto pairFound = texMap.find(filename); //Mira si existe ya una copia de este. 

	if (pairFound != texMap.end()) {
		//Devuelve la textura. 
		return pairFound->second;
	}
	else {
		//Crea el elemento. 
		auto& texture = texMap[filename];
		texture.loadFromFile(filename);
		return texture;
	}

}