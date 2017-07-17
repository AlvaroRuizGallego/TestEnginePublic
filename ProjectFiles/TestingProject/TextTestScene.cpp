#include "TextTestScene.h"

#include "TextLabel.h"
#include "SpriteRenderer.h"

#include "Drawer.h"



TextTestScene::TextTestScene()
{
}

TextTestScene::~TextTestScene()
{
}

void TextTestScene::onInitialize()
{
	//Camera

	GameObject* cameraGameObject = CreateGameObject();
	cameraGameObject->AddComponent(new Camera());


	//TEST
	_textGameObject = CreateGameObject();
	_textLabel = _textGameObject->AddComponent(new TextLabel("Hello I'm a test text 1.\nHello I'm a test text 2.", "FreeSans.ttf", true, 0.5, 0.65, glm::vec4(1, 1, 1, 1), TextLabel::TextHorizontalAlignment::H_LEFT, TextLabel::TextVerticalAlignment::V_BOTTOM));
}

void TextTestScene::onTerminate()
{
}

void TextTestScene::update(double deltaTime)
{
	Drawer::GetInstance()->DrawCross(glm::vec2(), 1000, false, { 1,1,1,0.75 });
	Drawer::GetInstance()->DrawCross(glm::vec2(), 20, false, { 1,0,0,0.75 });
}

void TextTestScene::fixedUpdate(float fixedTimestep)
{
}

void TextTestScene::onButtonDown(int button)
{
	static int i = 0;


	if (button == INPUT_KEY_ESCAPE)
	{
		TerminateGame();
	}

	if (button == INPUT_KEY_ENTER)
	{
		ChangeToScene("TitleSceneAudio");
	}

	if (button == INPUT_KEY_BACKSPACE)
	{
		ChangeToScene("BenchmarkScene");
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

void TextTestScene::onButtonUp(int button)
{
}

void TextTestScene::onMouseButtonDown(int button, glm::vec2 position, glm::vec2 worldPosition)
{
}

void TextTestScene::onMouseButtonUp(int button, glm::vec2 position, glm::vec2 worldPosition)
{
}

void TextTestScene::onMouseMove(glm::vec2 position, glm::vec2 worldPosition)
{
}
