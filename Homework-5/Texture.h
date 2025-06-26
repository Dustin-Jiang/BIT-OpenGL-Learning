#pragma once
#include "stdafx.h"
#include <atlimage.h>
#include <string>
#include <stdexcept>

// Helper to check for OpenGL errors
inline void getError() {
	GLenum err;
	if ((err = glGetError()) != GL_NO_ERROR) {
		std::string errorMsg = "OpenGL error: " + std::string((char*)gluErrorString(err));
		throw std::runtime_error(errorMsg);
	}
}

class Texture {
public:
	std::wstring fileName;
	GLuint id;

public:
	Texture(std::wstring fileName): fileName(fileName), id(0) {
		CImage img;
		HRESULT hResult = img.Load(fileName.data());
		if (FAILED(hResult))
		{
			throw std::runtime_error("Failed to load texture from file.");
		}
		
		// 检查图像格式是否支持
		if (img.GetBPP() != 24 && img.GetBPP() != 32) {
			throw std::runtime_error("Unsupported image format. Only 24-bit and 32-bit images are supported.");
		}
		
		// Create The Texture
		glGenTextures(1, &id);
		getError();
		glBindTexture(GL_TEXTURE_2D, id);
		getError();

		// Generate The Texture
		int pitch = img.GetPitch();
		GLenum format = (img.GetBPP() == 24) ? GL_BGR : GL_BGRA;
		GLenum internalFormat = (img.GetBPP() == 24) ? GL_RGB : GL_RGBA;
		
		if (pitch < 0)
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, img.GetWidth(), img.GetHeight(), 0, format, GL_UNSIGNED_BYTE, img.GetPixelAddress(0, img.GetHeight() - 1));
		else
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, img.GetWidth(), img.GetHeight(), 0, format, GL_UNSIGNED_BYTE, img.GetBits());
		getError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
		getError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtering
		getError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		getError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		getError();
	}
};