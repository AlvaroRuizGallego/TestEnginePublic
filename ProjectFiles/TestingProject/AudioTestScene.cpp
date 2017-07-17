#include "AudioTestScene.h"

#include "SpriteSheet.h"
#include "InputBinds.h"

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "AnimatedSpriteRenderer.h"
#include "Rigidbody.h"

#include "Drawer.h"

#include "TestStateMachine.h"

AudioTestScene::AudioTestScene()
{
}

AudioTestScene::~AudioTestScene()
{
}

void AudioTestScene::onInitialize()
{
	_audioSource = new AudioSource("raining.wav", true, true, 1, 1, 1);
	_audioSource->Play(1);

	_audioGameObject = CreateGameObject();
	_audioGameObject->AddComponent(_audioSource);
	_audioGameObject->AddComponent(new SpriteRenderer("circle.png"))->SetColor({ 0, 0.5, 1, 1 });
	_audioGameObject->SetScale(0.2, 0.2);
}

void AudioTestScene::onTerminate()
{
}

void AudioTestScene::update(double deltaTime)
{
	Drawer::GetInstance()->DrawCross(glm::vec2(), 10000, false);
	Drawer::GetInstance()->DrawCircle(vec2(), 256, { 0,1,0,1 }, 4);
	Drawer::GetInstance()->DrawCircle(vec2(), 512, { 0,1,0,1 }, 4);

	Drawer::GetInstance()->DrawLine({ -256, -1000 }, { -256, 1000 }, { 1, 0.1, 0, 1 }, 4);
	Drawer::GetInstance()->DrawLine({ 256, 1000 }, { 256, -1000 }, { 1, 0.1, 0, 1 }, 4);

	Drawer::GetInstance()->DrawLine({ -512, -1000 }, { -512, 1000 }, { 1, 0.1, 0, 1 }, 4);
	Drawer::GetInstance()->DrawLine({ 512, 1000 }, { 512, -1000 }, { 1, 0.1, 0, 1 }, 4);
}

void AudioTestScene::fixedUpdate(float fixedTimestep)
{
}

void AudioTestScene::onButtonDown(int button)
{
	if (button == INPUT_KEY_ESCAPE)
	{
		TerminateGame();
	}
	if (button == INPUT_KEY_R)
	{
		ChangeToScene("AudioTestScene");
	}

	if (button == INPUT_KEY_ENTER)
	{
		ChangeToScene("AudioTestScene2");
	}

	if (button == INPUT_KEY_BACKSPACE)
	{
		ChangeToScene("TitleSceneAudio");
	}

	if (button == INPUT_KEY_SPACE)
	{
		_audioSource->Pause(2);
	}
}

void AudioTestScene::onButtonUp(int button)
{
	if (button == INPUT_KEY_SPACE)
	{
		printf("UP");
		_audioSource->Play(2);
	}
}

void AudioTestScene::onMouseMove(glm::vec2 position, glm::vec2 worldPosition)
{
	_audioGameObject->SetPositionX(worldPosition.x);
	_audioGameObject->SetPositionY(worldPosition.y);
}
