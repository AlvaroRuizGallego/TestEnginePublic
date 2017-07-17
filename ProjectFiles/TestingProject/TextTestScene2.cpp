#include "TextTestScene2.h"

#include "TextLabel.h"
#include "SpriteRenderer.h"

#include "Drawer.h"



TextTestScene2::TextTestScene2()
{
}

TextTestScene2::~TextTestScene2()
{
}

void TextTestScene2::onInitialize()
{
	//Camera

	GameObject* cameraGameObject = CreateGameObject();
	cameraGameObject->AddComponent(new Camera());


	//TEST
	_textGameObject = CreateGameObject();
	_textLabel = _textGameObject->AddComponent(new TextLabel("A", "FreeSans.ttf", true, 1, 0.65, glm::vec4(1, 1, 1, 1), TextLabel::TextHorizontalAlignment::H_CENTER, TextLabel::TextVerticalAlignment::V_CENTER));
}

void TextTestScene2::onTerminate()
{
}

void TextTestScene2::update(double deltaTime)
{
}

void TextTestScene2::fixedUpdate(float fixedTimestep)
{
}

void TextTestScene2::onButtonDown(int button)
{
	static int i = 0;


	if (button == INPUT_KEY_ESCAPE)
	{
		TerminateGame();
	}

	if (button == INPUT_KEY_ENTER)
	{
		ChangeToScene("AudioTestScene");
	}
	if (button == INPUT_KEY_SPACE)
	{
		i++;

		if (i >= 5) i = 0;

		switch (i)
		{
			case 0:
				_textLabel->SetHorizontalAlignment(TextLabel::TextHorizontalAlignment::H_LEFT);
				_textLabel->SetVerticalAlignment(TextLabel::TextVerticalAlignment::V_BOTTOM);
				break;

			case 1:
				_textLabel->SetHorizontalAlignment(TextLabel::TextHorizontalAlignment::H_LEFT);
				_textLabel->SetVerticalAlignment(TextLabel::TextVerticalAlignment::V_TOP);
				break;

			case 2:
				_textLabel->SetHorizontalAlignment(TextLabel::TextHorizontalAlignment::H_RIGHT);
				_textLabel->SetVerticalAlignment(TextLabel::TextVerticalAlignment::V_TOP);
				break;

			case 3:
				_textLabel->SetHorizontalAlignment(TextLabel::TextHorizontalAlignment::H_RIGHT);
				_textLabel->SetVerticalAlignment(TextLabel::TextVerticalAlignment::V_BOTTOM);
				break;

			case 4:
				_textLabel->SetHorizontalAlignment(TextLabel::TextHorizontalAlignment::H_CENTER);
				_textLabel->SetVerticalAlignment(TextLabel::TextVerticalAlignment::V_CENTER);
				break;
		}
	}
}

void TextTestScene2::onButtonUp(int button)
{
}

void TextTestScene2::onMouseButtonDown(int button, glm::vec2 position, glm::vec2 worldPosition)
{
}

void TextTestScene2::onMouseButtonUp(int button, glm::vec2 position, glm::vec2 worldPosition)
{
}

void TextTestScene2::onMouseMove(glm::vec2 position, glm::vec2 worldPosition)
{
}
