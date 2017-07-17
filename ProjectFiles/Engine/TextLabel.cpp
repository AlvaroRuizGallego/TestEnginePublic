#include "TextLabel.h"

#include "_SpriteManager.h"
#include "_FontLoader.h"
#include "Camera.h"


void TextLabel::updateSprites()
{
	for (_Sprite* sprite : _characterSprites)
	{
		_SpriteManager::GetInstance()->RemoveSprite(sprite);
		delete sprite;
	}

	_characterSprites.clear();


	int cursor = 0;
	int baseline = 0;
	
	int firstSpriteIndex = 0;

	int top = 0;
	int bottom = 0;

	int spriteIndex = 0;
	for (int i = 0; i < _text.size(); ++i)
	{
		char character = _text[i];

		if (character == '\n')
		{
			alignSpritesHorizontally(firstSpriteIndex, cursor);

			cursor = 0;
			baseline -= _font->GetLineHeight() * _scale * _lineHeightScale;
			firstSpriteIndex = spriteIndex;
			
			continue;
		}


		_Sprite* sprite = new _Sprite(_font->GetTexture(), _cameraSpace, _font->GetUVs(character));
		sprite->SetParent(_gameObject);
		sprite->SetVisible(spriteIndex < _nVisibleCharacters);
		sprite->SetColor(_color);

		_characterSprites.push_back(sprite);

		_SpriteManager::GetInstance()->AddSprite(sprite);
		sprite->SetPosition(cursor + (_font->GetPosition(character).x + _font->GetSize(character).x * 0.5) * _scale, baseline + (_font->GetPosition(character).y - _font->GetSize(character).y * 0.5) * _scale);
		sprite->SetScale(_scale, _scale);

		int currentTop = baseline + _font->GetPosition(character).y * _scale;
		if (top < currentTop) top = currentTop;

		int currentBottom = currentTop - _font->GetSize(character).y * _scale;
		if (bottom > currentBottom) bottom = currentBottom;

		cursor += _font->GetAdvance(character) * _scale;

		spriteIndex++;
	}

	alignSpritesHorizontally(firstSpriteIndex, cursor);
	alignSpritesVertically(top, bottom);
}

void TextLabel::alignSpritesHorizontally(int spriteIndex, int lineWidth)
{
	float alignmentScale = _horizontalAlignment == TextHorizontalAlignment::H_LEFT ? 0 : _horizontalAlignment == TextHorizontalAlignment::H_CENTER ? 0.5 : 1;

	for (int i = spriteIndex; i < _characterSprites.size(); ++i)
	{
		_Sprite* sprite = _characterSprites[i];
		sprite->SetPosition(sprite->GetPosition().x - (float)lineWidth * alignmentScale, sprite->GetPosition().y);
	}
}

void TextLabel::alignSpritesVertically(int top, int bottom)
{
	int height = top - bottom;

	float alignmentScale = _verticalAlignment == TextVerticalAlignment::V_TOP ? 0 : _verticalAlignment == TextVerticalAlignment::V_CENTER ? 0.5 : 1;

	for (int i = 0; i < _characterSprites.size(); ++i)
	{
		_Sprite* sprite = _characterSprites[i];
		sprite->SetPosition(sprite->GetPosition().x, sprite->GetPosition().y - top + height * alignmentScale);
	}
}

TextLabel::TextLabel(std::string text, std::string fontFile, bool cameraSpace, float scale, float lineHeightScale, glm::vec4 color, TextHorizontalAlignment horizontalAlignment, TextVerticalAlignment verticalAlignment)
{
	_text = text;
	_font = _FontLoader::GetInstance()->LoadFont(fontFile);
	_scale = scale;
	_lineHeightScale = lineHeightScale;
	_color = color;
	_horizontalAlignment = horizontalAlignment;
	_verticalAlignment = verticalAlignment;
	_nVisibleCharacters = _text.size();
	_cameraSpace = cameraSpace;
}

TextLabel::~TextLabel()
{
}

void TextLabel::OnInitialize()
{
	updateSprites();
}

void TextLabel::OnTerminate()
{
}

std::string TextLabel::GetText()
{
	return _text;
}

void TextLabel::SetText(std::string text)
{
	_text = text;

	if (_nVisibleCharacters != _text.size()) _nVisibleCharacters = _text.size();

	updateSprites();
}


void TextLabel::SetFontFile(std::string fontFile)
{
	_font = _FontLoader::GetInstance()->LoadFont(fontFile);

	updateSprites();
}

float TextLabel::GetScale()
{
	return _scale;
}

void TextLabel::SetScale(float scale)
{
	_scale = scale;

	updateSprites();
}

float TextLabel::GetLineHeightScale()
{
	return _lineHeightScale;
}

void TextLabel::SetLineHeightScale(float scale)
{
	_lineHeightScale = scale;

	updateSprites();
}

TextLabel::TextHorizontalAlignment TextLabel::GetHorizontalAlignment()
{
	return _horizontalAlignment;
}

void TextLabel::SetHorizontalAlignment(TextHorizontalAlignment alignment)
{
	_horizontalAlignment = alignment;
	updateSprites();
}

TextLabel::TextVerticalAlignment TextLabel::GetVerticalAlignment()
{
	return _verticalAlignment;
}

void TextLabel::SetVerticalAlignment(TextVerticalAlignment alignment)
{
	_verticalAlignment = alignment;
	updateSprites();
}

int TextLabel::GetNVisibleCharacters()
{
	return _nVisibleCharacters;
}

void TextLabel::SetNVisibleCharacters(int n)
{
	_nVisibleCharacters = glm::clamp(n, 0, (int)_text.size());

	for (int i = 0; i < _characterSprites.size(); ++i)
	{
		_Sprite* sprite = _characterSprites[i];
		sprite->SetVisible(i < _nVisibleCharacters);
	}
}

void TextLabel::SetAllVisibleCharacters()
{
	SetNVisibleCharacters(_text.size());
}
