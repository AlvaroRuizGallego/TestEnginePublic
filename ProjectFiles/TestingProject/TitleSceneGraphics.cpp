#include "TitleSceneGraphics.h"
#include "TextLabel.h"


void TitleSceneGraphics::onInitialize()
{
	CreateGameObject()->AddComponent(new TextLabel("GRAPHICS", "Questrial-Regular.ttf", true, 1, 1, { 1,1,1,1 }, TextLabel::TextHorizontalAlignment::H_CENTER, TextLabel::TextVerticalAlignment::V_CENTER));
}

void TitleSceneGraphics::onButtonDown(int button)
{
	if (button == INPUT_KEY_ESCAPE)
	{
		TerminateGame();
	}

	if (button == INPUT_KEY_ENTER)
	{
		ChangeToScene("TransparencyTestScene");
	}

	if (button == INPUT_KEY_BACKSPACE)
	{
		ChangeToScene("TitleScene");
	}
}
