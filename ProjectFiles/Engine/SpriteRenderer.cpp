#include "SpriteRenderer.h"

#include "_SpriteManager.h"
#include "_Sprite.h"
#include "Drawer.h"

#include <algorithm>


SpriteRenderer::SpriteRenderer()
{
}

SpriteRenderer::SpriteRenderer(std::string imagePath, bool cameraSpace, Rectangle uvs)
{
	_sprite = new _Sprite(imagePath, cameraSpace, uvs);
}

SpriteRenderer::~SpriteRenderer()
{
	if (!_addedToScene)
	{
		delete _sprite;
	}
}

void SpriteRenderer::OnInitialize()
{
	_SpriteManager::GetInstance()->AddSprite(_sprite);
	_addedToScene = true;

	_sprite->SetParent(_gameObject);
}

void SpriteRenderer::OnTerminate()
{
	_SpriteManager::GetInstance()->RemoveSprite(_sprite);
	_addedToScene = false;

	_sprite->SetParent(nullptr);
}

glm::vec2 SpriteRenderer::GetSpriteSize()
{
	return _sprite->GetSize();
}

void SpriteRenderer::SetParallax(float parallax)
{
	_sprite->SetParallax(parallax);
}

float SpriteRenderer::GetParallax()
{
	return _sprite->GetParallax();
}

void SpriteRenderer::SetColor(glm::vec4 color)
{
	_sprite->SetColor(color);
}

glm::vec4 SpriteRenderer::GetColor()
{
	return _sprite->GetColor();;
}

void SpriteRenderer::SetVisible(bool visible)
{
	_sprite->SetVisible(visible);
}

bool SpriteRenderer::IsVisible()
{
	return _sprite->IsVisible();
}

void SpriteRenderer::SetUVs(Rectangle uvs)
{
	_sprite->SetUVs(uvs);
}

Rectangle SpriteRenderer::GetUVs()
{
	return _sprite->GetUVRectangle();
}

void SpriteRenderer::SetImagePath(string imagePath)
{
	_SpriteManager::GetInstance()->RemoveSprite(_sprite);

	_sprite->SetImagePath(imagePath);

	_SpriteManager::GetInstance()->AddSprite(_sprite);
}

string SpriteRenderer::GetImagePath()
{
	return _sprite->GetTexture()->GetImagePath();
}


