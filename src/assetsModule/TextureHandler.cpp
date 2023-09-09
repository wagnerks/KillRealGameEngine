﻿#include "TextureHandler.h"

#include <stb_image.h>
#include <string>

#include "glad/glad.h"
#include "logsModule/logger.h"

using namespace AssetsModule;

void TextureHandler::bindTexture(unsigned slot, unsigned type, unsigned id) {
	if (mBindedTextures[slot] == id) {
		return;
	}
	mBindedTextures[slot] = id;
	glActiveTexture(slot);
	glBindTexture(type, id);
}

Texture TextureLoader::loadTexture(const std::string& path, bool flip) {
	if (&TextureHandler::instance()->mLoader == this) {
		auto it = mLoadedTex.find(path);
		if (it != mLoadedTex.end()) {
			return it->second;
		}
		stbi_set_flip_vertically_on_load(flip);
		unsigned texID;

		int texWidth, texHeight, nrChannels;

		auto data = stbi_load(path.data(), &texWidth, &texHeight, &nrChannels, 4);
		if (!data) {
			Engine::LogsModule::Logger::LOG_ERROR("TextureHandler::can't load texture %s", path.c_str());
			stbi_image_free(data);
			return TextureHandler::instance()->mDefaultTex;
		}
		glGenTextures(1, &texID);

		TextureHandler::instance()->bindTexture(GL_TEXTURE0, GL_TEXTURE_2D, texID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

		Texture tex = { texID, path, eTextureType::DEFAULT };
		mLoadedTex[path] = tex;
		return tex;
	}

	auto it = mLoadedTex.find(path);
	if (it != mLoadedTex.end()) {
		return it->second;
	}
	auto id = TextureHandler::instance()->mLoader.loadTexture(path, flip);
	mLoadedTex[path] = id;
	return id;
}

Texture TextureLoader::loadCubemapTexture(const std::string& path, bool flip) {
	if (&TextureHandler::instance()->mLoader == this) {
		auto it = mLoadedTex.find(path);
		if (it != mLoadedTex.end()) {
			return it->second;
		}

		stbi_set_flip_vertically_on_load(flip);

		unsigned int textureID;
		glGenTextures(1, &textureID);
		TextureHandler::instance()->bindTexture(GL_TEXTURE0, GL_TEXTURE_CUBE_MAP, textureID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		std::vector<std::string> faces{
			path + "right.jpg",
				path + "left.jpg",
				path + "top.jpg",
				path + "bottom.jpg",
				path + "front.jpg",
				path + "back.jpg"
		};

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++) {
			auto data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (!data) {
				Engine::LogsModule::Logger::LOG_ERROR("TextureHandler::can't load texture %s", faces[i].c_str());
				stbi_image_free(data);
				glDeleteTextures(1, &textureID);
				return {};
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}

		Texture tex = { textureID, path, eTextureType::CUBEMAP };
		mLoadedTex[path] = tex;
		return tex;
	}

	auto it = mLoadedTex.find(path);
	if (it != mLoadedTex.end()) {
		return it->second;
	}
	auto id = TextureHandler::instance()->mLoader.loadCubemapTexture(path, flip);
	mLoadedTex[path] = id;
	return id;
}

Texture TextureLoader::createEmpty2DTexture(const std::string& id, int w, int h, int format) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	TextureHandler::instance()->bindTexture(GL_TEXTURE0, GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, nullptr);

	return { textureID, "", eTextureType::DEFAULT };
}
