#ifndef _SPRITE_BATCH_H
#define _SPRITE_BATCH_H

#include "_Texture.h"
#include "_Sprite.h"
#include "Geometry.h"

#include <vector>

class _SpriteBatch
{
	private:
		_Texture* _texture;
		std::vector<_Sprite*> _sprites;
		bool _sortNeeded;

	public:
		_SpriteBatch(_Texture* texture);
		_SpriteBatch(std::string  imagePath);
		~_SpriteBatch();

		void AddSprite(_Sprite* sprite);
		void RemoveSprite(_Sprite* sprite);

		void SetSortFlag();

		_Texture* GetTexture();
		std::vector<_Sprite*>* GetSpriteVector(bool sorted = false);
};

#endif
