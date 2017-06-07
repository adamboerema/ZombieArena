#include "stdafx.h"
#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::instance = nullptr;

TextureHolder::TextureHolder() {
	assert(instance == nullptr);
	instance = this;
}

Texture& TextureHolder::GetTexture(string const& filename) {

	auto& textureMap = instance->textures;
	auto file = textureMap.find(filename);

	if (file != textureMap.end()) {
		return file->second;
	}
	else {
		auto& texture = textureMap[filename];
		texture.loadFromFile(filename);
		return texture;
	}
}