#include "_Sprite.h"

#include "_SpriteManager.h"
#include "_Texture.h"
#include "Component.h"
#include "GameObject.h"

#include "gtc\matrix_transform.hpp"


_Sprite::_Sprite(std::string  imagePath, bool cameraSpace, Rectangle uvs)
{
	_texture = _SpriteManager::GetInstance()->GetTexture(imagePath);
	_uvs = uvs;

	_cameraSpace = cameraSpace;

	setup();
}

_Sprite::_Sprite(_Texture* texture, bool cameraSpace, Rectangle uvs)
{
	_texture = texture;
	_uvs = uvs;

	_cameraSpace = cameraSpace;

	_isTransformDirty = true;

	setup();
}

_Sprite::~_Sprite()
{
}

void _Sprite::setup()
{
	setSize(_texture->GetWidth() * _uvs.GetWidth(), _texture->GetHeight() * _uvs.GetHeight());
	_size.z = 1;

	SetPosition(0, 0);

	SetScale(1, 1);
	_scale.z = 1;

	SetOrientation(0);

	_parallax = 1;

	_visible = true;

	_color = glm::vec4(1, 1, 1, 1);


	_uvVector.push_back(_uvs.min.x);
	_uvVector.push_back(_uvs.min.y);
	_uvVector.push_back(_uvs.max.x);
	_uvVector.push_back(_uvs.min.y);
	_uvVector.push_back(_uvs.max.x);
	_uvVector.push_back(_uvs.max.y);
	_uvVector.push_back(_uvs.min.x);
	_uvVector.push_back(_uvs.max.y);
}

void _Sprite::setSize(float width, float height)
{
	_size.x = width;
	_size.y = height;

	_isTransformDirty = true;
}


void _Sprite::SetPosition(float x, float y)
{
	_position.x = x;
	_position.y = y;

	_isTransformDirty = true;
}

void _Sprite::SetScale(float x, float y)
{
	_scale.x = x;
	_scale.y = y;

	_isTransformDirty = true;
}

void _Sprite::SetOrientation(float radians)
{
	float pi2 = glm::pi<float>() * 2;

	if (radians > pi2)
	{
		radians -= pi2 * int(radians / pi2);
	}
	else if (radians < 0)
	{
		radians += pi2 * int(-radians / pi2) + pi2;
	}

	_orientation = radians;

	_isTransformDirty = true;
}

void _Sprite::SetDepth(float glZ)
{
	_position.z = glZ;

	_SpriteManager::GetInstance()->SetSpriteBatchSortFlag(this);

	_isTransformDirty = true;
}

void _Sprite::SetParallax(float parallax)
{
	_parallax = parallax;
}

void _Sprite::SetColor(glm::vec4 color)
{
	_color = color;
}

glm::vec2 _Sprite::GetSize()
{
	return glm::vec2(_size.x, _size.y);
}

glm::vec2 _Sprite::GetPosition()
{
	return glm::vec2(_position.x, _position.y);
}

glm::vec2 _Sprite::GetScale()
{
	return glm::vec2(_scale.x, _scale.y);
}

float _Sprite::GetOrientation()
{
	return _orientation;
}

float _Sprite::GetDepth()
{
	return _position.z;
}

float _Sprite::GetParallax()
{
	return _parallax;
}

glm::vec4 _Sprite::GetColor()
{
	return _color;
}

void _Sprite::SetVisible(bool visible)
{
	_isTransformDirty = _isTransformDirty ||_visible != visible;

	_visible = visible;
}

bool _Sprite::IsVisible()
{
	return _visible;
}

bool _Sprite::IsCameraSpace()
{
	return _cameraSpace;
}

glm::mat4 _Sprite::GetTransform()
{
	if (_isTransformDirty)
	{
		_transform = glm::mat4();
		_transform = glm::translate(_transform, _position);
		_transform = glm::rotate(_transform, _orientation, glm::vec3(0, 0, -1));
		_transform = glm::scale(_transform, _visible ? _size * _scale : glm::vec3());

		_isTransformDirty = false;
	}

	if (_parent != nullptr)
	{
		return _parent->GetTransform() * _transform;
	}

	return _transform;
}

void _Sprite::SetParent(GameObject * parent)
{
	_parent = parent;
}

GameObject * _Sprite::GetParent()
{
	return _parent;
}

_Texture * _Sprite::GetTexture()
{
	return _texture;
}

void _Sprite::SetImagePath(std::string imagePath)
{
	_texture = _SpriteManager::GetInstance()->GetTexture(imagePath);
	setSize(_texture->GetWidth() * _uvs.GetWidth(), _texture->GetHeight() * _uvs.GetHeight());
}

Rectangle _Sprite::GetUVRectangle()
{
	return _uvs;
}

void _Sprite::SetUVs(Rectangle uvs)
{
	_uvs = uvs;

	setSize(_texture->GetWidth() * uvs.GetWidth(), _texture->GetHeight() * uvs.GetHeight());

	_uvVector[0] = _uvs.min.x;
	_uvVector[1] = _uvs.min.y;
	_uvVector[2] = _uvs.max.x;
	_uvVector[3] = _uvs.min.y;
	_uvVector[4] = _uvs.max.x;
	_uvVector[5] = _uvs.max.y;
	_uvVector[6] = _uvs.min.x;
	_uvVector[7] = _uvs.max.y;
}

const std::vector<float>* _Sprite::GetUVs()
{
	return &_uvVector;
}


