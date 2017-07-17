#include "SpriteSheetManager.h"


SpriteSheetManager* SpriteSheetManager::_instance = nullptr;

SpriteSheetManager::SpriteSheetManager()
{
}


SpriteSheetManager::~SpriteSheetManager()
{
	_instance = nullptr;
}

SpriteSheetManager * SpriteSheetManager::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new SpriteSheetManager();
	}

	return _instance;
}

SpriteSheet * SpriteSheetManager::GetSpriteSheet(std::string spriteSheetFile)
{
	auto it = _spriteSheetMap.find(spriteSheetFile);
	if (it != _spriteSheetMap.end())
	{
		return it->second;
	}
	else
	{
		SpriteSheet* spriteSheet = new SpriteSheet(spriteSheetFile.c_str());
		_spriteSheetMap[spriteSheetFile] = spriteSheet;

		return spriteSheet;
	}
}

void SpriteSheetManager::Clear()
{
	for (auto it = _spriteSheetMap.begin(); it != _spriteSheetMap.end(); ++it)
	{
		delete it->second;
	}

	_spriteSheetMap.clear();
}
