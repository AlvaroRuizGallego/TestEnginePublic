#include "_Texture.h"

#include <FreeImage.h>

#include "glm.hpp"

_Texture* _Texture::_emptyTexture = nullptr;

_Texture::_Texture()
{
	_width = _height = 0;
	_pixels = nullptr;
	_imagePath = "";
	_empty = true;
}

void _Texture::readImageFile(std::string  imagePath)
{
	FIBITMAP* baseImage = FreeImage_Load(FreeImage_GetFileType(imagePath.c_str(), 0), imagePath.c_str());
	FIBITMAP* image = FreeImage_ConvertTo32Bits(baseImage);

	_width = FreeImage_GetWidth(image);
	_height = FreeImage_GetHeight(image);

	if (_width == 0 || _height == 0)
	{
		printf("\n_Texture::readImageFile -> Image (%s) not loaded properly.", imagePath.c_str());
		_empty = true;
	}

	_pixels = new GLubyte[4 * _width * _height]();


	GLubyte* tempPixels = (GLubyte*)FreeImage_GetBits(image);

	//Swap red and blue colors
	int posPixels;
	for (int i = _height - 1; i >= 0; --i)
	{
		for (int j = _width - 1; j >= 0; --j)
		{
			posPixels = (i * _width + j) * 4;

			_pixels[posPixels + 0] = tempPixels[posPixels + 2];
			_pixels[posPixels + 1] = tempPixels[posPixels + 1];
			_pixels[posPixels + 2] = tempPixels[posPixels + 0];
			_pixels[posPixels + 3] = tempPixels[posPixels + 3];
		}
	}

	//[0,0] -> bottom left

	FreeImage_Unload(baseImage);
	FreeImage_Unload(image);

	fixBorderColors();
}

void _Texture::fixBorderColors()
{
	//Swap red and blue colors
	int posPixels;
	for (int row = _height - 1; row >= 0; --row)
	{
		for (int column = _width - 1; column >= 0; --column)
		{
			posPixels = (row * _width + column) * 4;
			
			if (_pixels[posPixels + 3] == 0)
			{
				glm::vec3 finalColor = glm::vec3();
				int nNeighbours = 0;

				auto addNeighbourColor = [&](int c, int r)->void
				{
					int posPixelsNeighbour = (r * _width + c) * 4;

					if (_pixels[posPixelsNeighbour + 3] != 0)
					{
						finalColor += glm::vec3(_pixels[posPixelsNeighbour + 0], _pixels[posPixelsNeighbour + 1], _pixels[posPixelsNeighbour + 2]);
						nNeighbours++;
					}
				};


				//Left
			
				if (column > 0)
				{
					addNeighbourColor(column - 1, row);
				}

				//Left Up
				if (column > 0 && row < _height-1)
				{
					addNeighbourColor(column - 1, row + 1);
				}

				//Up
				if (row < _height - 1)
				{
					addNeighbourColor(column, row + 1);
				}

				//Right Up
				if (column < _width - 1 && row < _height - 1)
				{
					addNeighbourColor(column + 1, row + 1);
				}

				//Right
				if (column < _width - 1)
				{
					addNeighbourColor(column + 1, row);
				}

				//Right Bottom
				if (column < _width - 1 && row > 0)
				{
					addNeighbourColor(column + 1, row - 1);
				}

				//Bottom
				if (row > 0)
				{
					addNeighbourColor(column, row - 1);
				}

				//Left Bottom
				if (column > 0 && row > 0)
				{
					addNeighbourColor(column - 1, row - 1);
				}


				//Set final color

				if (nNeighbours != 0)
				{
					finalColor /= nNeighbours;
				}
				else
				{
					finalColor = { 0,0,0 };
				}

				_pixels[posPixels + 0] = finalColor.x;
				_pixels[posPixels + 1] = finalColor.y;
				_pixels[posPixels + 2] = finalColor.z;
				_pixels[posPixels + 3] = 0;
			}
		}
	}
}

_Texture::_Texture(std::string  imagePath)
{
	readImageFile(imagePath);
	_imagePath = imagePath;
}

_Texture::_Texture(GLubyte * pixels, GLsizei width, GLsizei height)
{
	_width = width;
	_height = height;

	if (_width == 0 || _height == 0)
	{
		_empty = true;
	}

	_pixels = pixels;

	_imagePath = "";
}

_Texture::~_Texture()
{
	delete[] _pixels;
}

GLubyte * _Texture::GetPixels()
{
	return _pixels;
}

GLsizei _Texture::GetWidth()
{
	return _width;
}

GLsizei _Texture::GetHeight()
{
	return _height;
}

GLint _Texture::GetFormat()
{
	return _format;
}

GLenum _Texture::GetDataType()
{
	return _dataType;
}

std::string _Texture::GetImagePath()
{
	return _imagePath;
}

bool _Texture::IsEmpty()
{
	return _empty;
}

_Texture * _Texture::GetEmptyTexture()
{
	if (_emptyTexture == nullptr)
	{
		_emptyTexture = new _Texture();
	}
	return _emptyTexture;
}


