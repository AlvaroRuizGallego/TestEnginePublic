#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "glfw3.h"
#include <string>


class _Texture
{
	private:
		//Texture info
		static const GLint _format = GL_RGBA;
		static const GLenum _dataType = GL_UNSIGNED_BYTE;

		static _Texture* _emptyTexture;

		GLubyte* _pixels;
		GLsizei _width, _height;
		std::string _imagePath;
		bool _empty = false;

		//Read image from file
		_Texture();

		void readImageFile(std::string  imagePath);
		void fixBorderColors();

	public:
		_Texture(std::string  imagePath);
		_Texture(GLubyte* pixels, GLsizei width, GLsizei height);
		~_Texture();

		//Getters
		GLubyte* GetPixels();
		GLsizei GetWidth();
		GLsizei GetHeight();
		static GLint GetFormat();
		static GLenum GetDataType();
		std::string GetImagePath();

		bool IsEmpty();

		static _Texture* GetEmptyTexture();
};

#endif

