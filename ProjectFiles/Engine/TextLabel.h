#ifndef TEXT_LABEL_H
#define TEXT_LABEL_H

#include "Component.h"
#include "_Sprite.h"

class _Font;


class __declspec(dllexport) TextLabel: public Component
{
	public:
		enum TextHorizontalAlignment { H_LEFT, H_CENTER, H_RIGHT };
		enum TextVerticalAlignment { V_TOP, V_CENTER, V_BOTTOM };

	private:
		_Font* _font;
		float _scale;
		float _lineHeightScale;
		std::string _text;

		bool _cameraSpace;

		glm::vec4 _color;

		std::vector<_Sprite*> _characterSprites;

		TextHorizontalAlignment _horizontalAlignment;
		TextVerticalAlignment _verticalAlignment;

		int _nVisibleCharacters;

		void updateSprites();
		void alignSpritesHorizontally(int spriteIndex, int lineWidth);
		void alignSpritesVertically(int top, int bottom);

	public:
		TextLabel(std::string text, std::string fontFile, bool cameraSpace = false, float scale = 1, float lineHeightScale = 1, glm::vec4 color = glm::vec4(1, 1, 1, 1), TextHorizontalAlignment horizontalAlignment = TextHorizontalAlignment::H_LEFT, TextVerticalAlignment verticalAlignment = TextVerticalAlignment::V_TOP);
		~TextLabel();

		void OnInitialize() override;
		void OnTerminate() override;

		std::string GetText();
		void SetText(std::string text);

		void SetFontFile(std::string fontFile);

		float GetScale();
		void SetScale(float scale);

		float GetLineHeightScale();
		void SetLineHeightScale(float scale);

		TextHorizontalAlignment GetHorizontalAlignment();
		void SetHorizontalAlignment(TextHorizontalAlignment alignment);

		TextVerticalAlignment GetVerticalAlignment();
		void SetVerticalAlignment(TextVerticalAlignment alignment);

		int GetNVisibleCharacters();
		void SetNVisibleCharacters(int n);
		void SetAllVisibleCharacters();
};

#endif

