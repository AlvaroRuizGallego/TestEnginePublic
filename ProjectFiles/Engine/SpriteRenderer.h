#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Component.h"
#include "Geometry.h"

class _Sprite;

class __declspec(dllexport) SpriteRenderer: public Component
{
	protected:
		_Sprite* _sprite;
		bool _addedToScene = false;

	public:
		SpriteRenderer();
		SpriteRenderer(std::string  imagePath, bool cameraSpace = false, Rectangle uvs = Rectangle({ 0,0 }, { 1,1 }));

		~SpriteRenderer();

		virtual void OnInitialize() override;
		void OnTerminate() override;

		glm::vec2 GetSpriteSize();

		void SetParallax(float parallax);
		float GetParallax();

		void SetColor(glm::vec4 color);
		glm::vec4 GetColor();

		void SetVisible(bool visible);
		bool IsVisible();

		void SetUVs(Rectangle uvs);
		Rectangle GetUVs();

		void SetImagePath(string imagePath);
		string GetImagePath();

};

#endif
