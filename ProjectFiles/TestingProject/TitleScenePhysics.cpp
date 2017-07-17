#include "TitleScenePhysics.h"
#include "TextLabel.h"


void TitleScenePhysics::onInitialize()
{
	CreateGameObject()->AddComponent(new TextLabel("PHYSICS", "Questrial-Regular.ttf", true, 1, 1, { 1,1,1,1 }, TextLabel::TextHorizontalAlignment::H_CENTER, TextLabel::TextVerticalAlignment::V_CENTER));
}

void TitleScenePhysics::onButtonDown(int button)
{
	if (button == INPUT_KEY_ESCAPE)
	{
		TerminateGame();
	}

	if (button == INPUT_KEY_ENTER)
	{
		ChangeToScene("CollisionTestScene");
	}

	if (button == INPUT_KEY_BACKSPACE)
	{
		ChangeToScene("AudioTestScene2");
	}
}
