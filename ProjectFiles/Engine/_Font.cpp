#include "_RenderingEngine.h"
#include "_Font.h"

#include "glm.hpp"

#include <iostream>



const std::string _Font::AVAILABLE_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ,.:!?()\n+-/*'";


void _Font::createTexture()
{
	FT_GlyphSlot glyph = _ftFace->glyph;

	int maxCharacterWidth = 0;
	int maxCharacterHeight = 0;

	for (int i = 0; i < AVAILABLE_CHARS.size(); ++i)
	{
		char character = AVAILABLE_CHARS[i];
		if (FT_Load_Char(_ftFace, character, FT_LOAD_RENDER))
		{
			printf("\nWARNING: _Font::createTexture -> Could not load character %s", character);
			continue;
		}

		int characterWidth = glyph->bitmap.width;
		int characterHeight = glyph->bitmap.rows;

		if (characterWidth > maxCharacterWidth) maxCharacterWidth = characterWidth;
		if (characterHeight > maxCharacterHeight) maxCharacterHeight = characterHeight;
	}

	float sqrtCharN = glm::sqrt(AVAILABLE_CHARS.size());

	int nRows, nColumns;
	nRows = nColumns = (sqrtCharN - (int)sqrtCharN) > 0 ? (int)sqrtCharN + 1 : (int)sqrtCharN;

	int textureWidth = nColumns * maxCharacterWidth;
	int textureHeight = nRows * maxCharacterHeight;

	GLubyte* texturePixels = new GLubyte[textureWidth * textureHeight * 4]();
	Rectangle uvs;

	for (int i = 0; i < AVAILABLE_CHARS.size(); ++i)
	{
		char character = AVAILABLE_CHARS[i];
		if (FT_Load_Char(_ftFace, character, FT_LOAD_RENDER))
		{
			printf("\nWARNING: _Font::createTexture -> Could not load character %s", character);
			continue;
		}

		int characterWidth = glyph->bitmap.width;
		int characterHeight = glyph->bitmap.rows;

		int currentRow = (int)(i / nColumns);
		int currentColumn = i - currentRow * nColumns;

		uvs.min = glm::vec2((float)currentColumn * (float)maxCharacterWidth / (float)textureWidth, (float)currentRow * (float)maxCharacterHeight / (float)textureHeight);
		uvs.max = glm::vec2(((float)currentColumn * (float)maxCharacterWidth + (float)characterWidth) / (float)textureWidth, ((float)currentRow * (float)maxCharacterHeight + (float)characterHeight) / (float)textureHeight);

		blitCharacter(texturePixels, glyph->bitmap.buffer, uvs, textureWidth, textureHeight, characterWidth, characterHeight);

		_characterMap[character] = _CharacterInfo{ uvs,  glm::vec2(glyph->bitmap.width, glyph->bitmap.rows), glm::vec2(glyph->bitmap_left, glyph->bitmap_top), glyph->advance.x / 64 };
	}

	_characterSheet = new _Texture(texturePixels, textureWidth, textureHeight);
}


void _Font::blitCharacter(GLubyte * texturePixels, GLubyte * characterPixels, Rectangle uvs, int textureWidth, int textureHeight, int characterWidth, int characterHeight)
{
	int basePosX = uvs.min.x * textureWidth;
	int basePosY = uvs.max.y * textureHeight -1;

	for (int y = 0; y < characterHeight; y++)
	{
		for (int x = 0; x < characterWidth; x++)
		{
			int texturePixelPos = ((basePosY - y) * textureWidth + basePosX + x) * 4;
			int characterPixelPos = (y * characterWidth + x);

			texturePixels[texturePixelPos + 0] = 255;
			texturePixels[texturePixelPos + 1] = 255;
			texturePixels[texturePixelPos + 2] = 255;
			texturePixels[texturePixelPos + 3] = characterPixels[characterPixelPos];
		}
	}
}

_Font::_Font()
{
}

_Font::_Font(FT_Face face)
{
	_ftFace = face;

	createTexture();
}


_Font::~_Font()
{
}

_Texture * _Font::GetTexture()
{
	return _characterSheet;
}

Rectangle _Font::GetUVs(char character)
{
	auto it = _characterMap.find(character);
	if (it != _characterMap.end())
	{
		return _characterMap.at(character).uvs;
	}
	else
	{
		printf("\nWARNING: _Font::GetUVs -> _characterMap doesn't contain key "); std::cout << character;
		return Rectangle();
	}
}

glm::vec2 _Font::GetSize(char character)
{
	auto it = _characterMap.find(character);
	if (it != _characterMap.end())
	{
		return _characterMap.at(character).size;
	}
	else
	{
		printf("\nWARNING: _Font::GetSize -> _characterMap doesn't contain key "); std::cout << character;
		return glm::vec2();
	}
}

glm::vec2 _Font::GetPosition(char character)
{
	auto it = _characterMap.find(character);
	if (it != _characterMap.end())
	{
		return _characterMap.at(character).position;
	}
	else
	{
		printf("\nWARNING: _Font::GetPosition -> _characterMap doesn't contain key "); std::cout << character;
		return glm::vec2();
	}
}

long _Font::GetAdvance(char character)
{
	auto it = _characterMap.find(character);
	if (it != _characterMap.end())
	{
		return _characterMap.at(character).advance;
	}
	else
	{
		printf("\nWARNING: _Font::GetAdvanceVector -> _characterMap doesn't contain key "); std::cout << character;
		return 0.0;
	}
}

int _Font::GetLineHeight()
{
	return _ftFace->size->metrics.height / 64;
}