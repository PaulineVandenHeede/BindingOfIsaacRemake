#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"
#include <iostream>

TextureManager::TextureManager()
	: m_Textures{}
{

}

TextureManager::~TextureManager()
{
	for (const std::pair<std::string, Texture*>& p : m_Textures)
	{
		delete p.second;
		// p.second = nullptr;
	}
	m_Textures.clear();
}

Texture* TextureManager::GetTexture(const std::string& filename)
{
	// end operator is returned when the filename is not found
	if (m_Textures.find(filename) == m_Textures.end())
	{
		m_Textures[filename] = new Texture{ filename };
		std::cout << "File loaded with name " << filename << "\n";
	}
	// here I am sure the key exists
	return m_Textures[filename];
}
