#pragma once

#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <SFML\Graphics.hpp>
#include <map>

using namespace sf;
using namespace std;

class TextureHolder {
private: 
	map<string, Texture> textures;
	static TextureHolder *instance;

public:
	TextureHolder();
	static Texture& GetTexture(String const& filename);
};

#endif // TEXTURE_HOLDER_H
