#include "_RenderingEngine.h"

#include "_SpriteBatch.h"

#include "TextLabel.h" //TEMP

#include <algorithm>

_SpriteBatch::_SpriteBatch(_Texture * texture)
{
	_texture = texture;
	_RenderingEngine::GetInstance()->LoadTexture(_texture);
	
	SetSortFlag();
}

_SpriteBatch::_SpriteBatch(std::string  imagePath)
{
	_texture = new _Texture(imagePath);
	_RenderingEngine::GetInstance()->LoadTexture(_texture);

	SetSortFlag();
}

_SpriteBatch::~_SpriteBatch()
{
	for (_Sprite* sprite : _sprites)
	{
		delete sprite;
	}

	_sprites.clear();

	delete _texture;
}

void _SpriteBatch::AddSprite(_Sprite * sprite)
{
	_sprites.push_back(sprite);

	SetSortFlag();
}

void _SpriteBatch::RemoveSprite(_Sprite * sprite)
{
	_sprites.erase(std::remove(_sprites.begin(), _sprites.end(), sprite), _sprites.end());
}

void _SpriteBatch::SetSortFlag()
{
	_sortNeeded = true;
}

_Texture * _SpriteBatch::GetTexture()
{
	return _texture;
}

std::vector<_Sprite*> * _SpriteBatch::GetSpriteVector(bool sorted)
{
	if (sorted && _sprites.size() > 1 && _sortNeeded)
	{
		std::sort(_sprites.begin(), _sprites.end(), [](_Sprite* sp1, _Sprite* sp2) -> bool 
		{
			float sp1ParentDepth = sp1->GetParent() == nullptr ? 0 : sp1->GetParent()->GetPosition().z;
			float sp2ParentDepth = sp2->GetParent() == nullptr ? 0 : sp2->GetParent()->GetPosition().z;

			return ((sp1->GetDepth() + sp1ParentDepth) > (sp2->GetDepth() + sp2ParentDepth));
		});

		_sortNeeded = false;
	}

	return &_sprites;
}

