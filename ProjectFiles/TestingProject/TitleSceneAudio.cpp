#include "TitleSceneAudio.h"
#include "TextLabel.h"


void TitleSceneAudio::onInitialize()
{
	CreateGameObject()->AddComponent(new TextLabel("AUDIO", "Questrial-Regular.ttf", true, 1, 1, { 1,1,1,1 }, TextLabel::TextHorizontalAlignment::H_CENTER, TextLabel::TextVerticalAlignment::V_CENTER));
}

void TitleSceneAudio::onButtonDown(int button)
{
	if (button == INPUT_KEY_ESCAPE)
	{
		TerminateGame();
	}

	if (button == INPUT_KEY_ENTER)
	{
		ChangeToScene("AudioTestScene");
	}

	if (button == INPUT_KEY_BACKSPACE)
	{
		ChangeToScene("TextTestScene");
	}
}
