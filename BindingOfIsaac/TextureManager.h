#pragma once
#include <unordered_map>

class Texture;


class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	// rule of 5
	TextureManager(const TextureManager& other) = delete;
	TextureManager(TextureManager&& other) = delete;
	TextureManager& operator=(const TextureManager& other) = delete;
	TextureManager& operator=(TextureManager&& other) = delete;


	Texture* GetTexture(const std::string& filename); 

private:
	std::unordered_map<std::string, Texture *> m_Textures;
};

