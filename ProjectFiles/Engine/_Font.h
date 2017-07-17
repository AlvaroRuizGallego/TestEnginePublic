#ifndef _FONT_H
#define _FONT_H

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Geometry.h"
#include "_Texture.h"
#include "_Sprite.h"

#include <map>


struct _CharacterInfo
{
	Rectangle uvs;
	glm::vec2 size;
	glm::vec2 position; //Relative to cursor & baseline (0,0) -> bottom left
	long advance;
};

class _Font
{
	friend class _FontLoader;

	static const std::string AVAILABLE_CHARS;

	private:

		FT_Face _ftFace;
		std::map<char, _CharacterInfo> _characterMap;
		_Texture* _characterSheet;


		void createTexture();
		void blitCharacter(GLubyte* texturePixels, GLubyte * characterPixels, Rectangle uvs, int textureWidth, int textureHeight, int characterWidth, int characterHeight);


	public:
		_Font();
		_Font(FT_Face face);
		~_Font();

		_Texture* GetTexture();
		Rectangle GetUVs(char character);
		glm::vec2 GetSize(char character);
		glm::vec2 GetPosition(char character);
		long GetAdvance(char character);
		int GetLineHeight();

};

#endif
