#include "TitleScene.h"
#include "TextLabel.h"


void TitleScene::onInitialize()
{
	CreateGameObject()->AddComponent(new TextLabel("2D GAME ENGINE\nDEMO", "Questrial-Regular.ttf", true, 1, 1, { 1,1,1,1 }, TextLabel::TextHorizontalAlignment::H_CENTER, TextLabel::TextVerticalAlignment::V_CENTER));
}

void TitleScene::onButtonDown(int button)
{
	if (button == INPUT_KEY_ESCAPE)
	{
		TerminateGame();
	}

	if (button == INPUT_KEY_ENTER)
	{
		ChangeToScene("TitleSceneGraphics");
	}

	if (button == INPUT_KEY_BACKSPACE)
	{
		ChangeToScene("ForcesTestScene");
	}
}
