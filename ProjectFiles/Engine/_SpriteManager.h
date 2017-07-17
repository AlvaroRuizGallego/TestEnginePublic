#ifndef _SPRITE_MANAGER_H
#define _SPRITE_MANAGER_H

#include "_SpriteBatch.h"

#include "glm.hpp"

class _SpriteManager
{
	private:
		static _SpriteManager* _instance;

		std::vector<_SpriteBatch*> _spriteBatches;
		std::vector<_Sprite*> _sprites;

		_SpriteManager();

		int getSpriteIndex(_Sprite* sprite);

		_SpriteBatch* getSpriteBatch(_Texture* texture);
		_SpriteBatch* getSpriteBatch(std::string  imagePath);
		_SpriteBatch* getSpriteBatch(_Sprite* sprite);


	public:
		~_SpriteManager();

		static _SpriteManager* GetInstance();

		void Clear();

		void AddSprite(_Sprite* sprite);

		void RemoveSprite(_Sprite * sprite);
		void RemoveSprite(int index);

		void SetSpriteBatchSortFlag(_Sprite* sprite);

		std::vector<_SpriteBatch*>* GetSpriteBatchVector();
		GLsizei GetSpriteCount();
		_Texture* GetTexture(std::string  imagePath);
};

#endif

