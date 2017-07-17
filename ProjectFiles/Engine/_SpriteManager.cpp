#include "_SpriteManager.h"

#include "_Texture.h"

#include <algorithm>


_SpriteManager* _SpriteManager::_instance = nullptr;

_SpriteManager::_SpriteManager()
{
}

_SpriteManager::~_SpriteManager()
{
	_instance = nullptr;
}

int _SpriteManager::getSpriteIndex(_Sprite * sprite)
{
	std::vector<_Sprite*>::iterator iter = std::find(_sprites.begin(), _sprites.end(), sprite);
	size_t index = std::distance(_sprites.begin(), iter);
	if (index == _sprites.size())
	{
		return _sprites.size();
	}
	return index;
}

_SpriteBatch * _SpriteManager::getSpriteBatch(_Texture* texture)
{
	if (texture->IsEmpty()) return nullptr;

	for (_SpriteBatch* spriteBatch : _spriteBatches)
	{
		if (spriteBatch->GetTexture() == texture)
		{
			return spriteBatch;
		}
	}

	_SpriteBatch * spriteBatch = new _SpriteBatch(texture);
	_spriteBatches.push_back(spriteBatch);

	return spriteBatch;
}

_SpriteBatch * _SpriteManager::getSpriteBatch(std::string  imagePath)
{
	for (_SpriteBatch* spriteBatch : _spriteBatches)
	{
		if (spriteBatch->GetTexture()->GetImagePath() == imagePath)
		{
			return spriteBatch;
		}
	}

	_SpriteBatch * spriteBatch = new _SpriteBatch(imagePath);

	if (spriteBatch->GetTexture()->IsEmpty())
	{
		delete spriteBatch;
		return nullptr;
	}

	_spriteBatches.push_back(spriteBatch);

	return spriteBatch;
}

_SpriteBatch * _SpriteManager::getSpriteBatch(_Sprite * sprite)
{
	return getSpriteBatch(sprite->_texture);
}

_SpriteManager * _SpriteManager::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new _SpriteManager();
	}
	return _instance;
}

void _SpriteManager::AddSprite(_Sprite * sprite)
{
	if (std::find(_sprites.begin(), _sprites.end(), sprite) == _sprites.end())
	{
		_SpriteBatch* spriteBatch = getSpriteBatch(sprite);
		if (spriteBatch != nullptr)
		{
			spriteBatch->AddSprite(sprite);
			_sprites.push_back(sprite);
		}
	}
	else
	{
		printf("\nWARNING: _SpriteManager::AddSprite -> Sprite already added");
	}
}

void _SpriteManager::RemoveSprite(_Sprite * sprite)
{
	int index = getSpriteIndex(sprite);
	if (index != _sprites.size())
	{
		RemoveSprite(index);
	}
	else
	{
		printf("\nWARNING: _SpriteManager::RemoveSprite -> Can't remove sprite, sprite not added");
	}
}

void _SpriteManager::RemoveSprite(int index)
{
	if (index >= _sprites.size() || index < 0)
	{
		printf("\nWARNING: _SpriteManager::RemoveSprite ->  Can't remove sprite, index not valid");
		return;
	}

	_Sprite* sprite = _sprites[index];

	getSpriteBatch(sprite->_texture)->RemoveSprite(sprite);
	_sprites.erase(_sprites.begin() + index);
}

void _SpriteManager::SetSpriteBatchSortFlag(_Sprite * sprite)
{
	_SpriteBatch* spriteBatch = getSpriteBatch(sprite);
	if (spriteBatch != nullptr)
	{
		spriteBatch->SetSortFlag();
	}
}



void _SpriteManager::Clear()
{
	for (_SpriteBatch* spriteBatch : _spriteBatches)
	{
		delete spriteBatch;
	}

	_spriteBatches.clear();
	_sprites.clear();
}

std::vector<_SpriteBatch*>* _SpriteManager::GetSpriteBatchVector()
{
	return &_spriteBatches;
}

GLsizei _SpriteManager::GetSpriteCount()
{
	return _sprites.size();
}

_Texture * _SpriteManager::GetTexture(std::string  imagePath)
{
	_SpriteBatch* spriteBatch = getSpriteBatch(imagePath);

	if (spriteBatch == nullptr)
	{
		return _Texture::GetEmptyTexture();
	}

	return spriteBatch->GetTexture();
}
