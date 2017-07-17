#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include "Geometry.h"

#include <string>
#include <map>
#include <vector>

class __declspec(dllexport) SpriteSheet
{
	friend class SpriteSheetManager;

	private:
		std::string _spriteSheetImageName;
		std::string _spriteSheetImagePath;
		std::map<std::string, Rectangle> _spriteMap;

		SpriteSheet(std::string spriteSheetFile);
		~SpriteSheet();
		void parseSpriteSheetData(std::string fileName);

	public:

		std::string GetSpriteSheetImageName();
		std::string GetSpriteSheetImagePath();
		Rectangle GetUVs(std::string spriteName);
		std::vector<Rectangle> GetAnimationUVs(std::string baseAnimationName);
};

#endif
