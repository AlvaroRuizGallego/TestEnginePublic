#ifndef SPRITE_SHEET_MANAGER_H
#define SPRITE_SHEET_MANAGER_H

#include "SpriteSheet.h"

#include <map>
#include <string>

class __declspec(dllexport) SpriteSheetManager
{
	private:
		static SpriteSheetManager* _instance;

		std::map<std::string, SpriteSheet*> _spriteSheetMap;

		SpriteSheetManager();

	public:
		~SpriteSheetManager();

		static SpriteSheetManager* GetInstance();

		SpriteSheet* GetSpriteSheet(std::string spriteSheetFile);

		void Clear();
};

#endif
