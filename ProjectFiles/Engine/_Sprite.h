#ifndef _SPRITE_H
#define _SPRITE_H

#include "Geometry.h"
#include "glm.hpp"

#include <vector>

class _Texture;
class Component;
class GameObject;

class _Sprite
{
	friend class _SpriteManager;

	private:
		glm::vec3 _size;
		glm::vec3 _position;
		glm::vec3 _scale;
		float _orientation;

		float _parallax;
		bool _visible;
		bool _cameraSpace;
		bool _isTransformDirty;

		glm::mat4 _transform;
		glm::vec4 _color;

		_Texture* _texture;
		Rectangle _uvs;
		GameObject* _parent = nullptr;

		std::vector<float> _uvVector;

		void setup();
		void setSize(float width, float height);

	public:

		_Sprite(std::string  imagePath, bool cameraSpace = false, Rectangle uvs = Rectangle({ 0,0 }, { 1,1 }));
		_Sprite(_Texture* texture, bool cameraSpace = false, Rectangle uvs = Rectangle({ 0,0 }, { 1,1 }));
		~_Sprite();

		void SetPosition(float x, float y);
		void SetScale(float x, float y);
		void SetOrientation(float radians);
		void SetDepth(float glZ);
		void SetParallax(float parallax);
		void SetColor(glm::vec4 color);
		void SetUVs(Rectangle uvs);

		glm::vec2 GetSize();
		glm::vec2 GetPosition();
		glm::vec2 GetScale();
		float GetOrientation();
		float GetDepth();
		float GetParallax();
		glm::vec4 GetColor();
		const std::vector<float>* GetUVs();
		Rectangle GetUVRectangle();

		void SetVisible(bool visible);
		bool IsVisible();

		bool IsCameraSpace();

		glm::mat4 GetTransform();

		void SetParent(GameObject* parent);
		GameObject* GetParent();

		_Texture* GetTexture();

		void SetImagePath(std::string imagePath);
};

#endif
