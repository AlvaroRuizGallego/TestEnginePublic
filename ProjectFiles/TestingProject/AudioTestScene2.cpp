#include "AudioTestScene2.h"

#include "SpriteSheet.h"
#include "InputBinds.h"

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "AnimatedSpriteRenderer.h"
#include "Rigidbody.h"

#include "AudioListener.h"
#include "CameraManualController.h"

#include "Drawer.h"

#include "TestStateMachine.h"

AudioTestScene2::AudioTestScene2()
{
}

AudioTestScene2::~AudioTestScene2()
{
}

void AudioTestScene2::onInitialize()
{
	_cameraGameObject = CreateGameObject();
	_cameraGameObject->AddComponent(new Camera());
	_cameraGameObject->AddComponent(new AudioListener());
	_cameraGameObject->AddComponent(new CameraManualController(250));


	GameObject* rainGameObject = CreateGameObject();
	rainGameObject->AddComponent(new AudioSource("raining.wav", true, true, 1, 1, 1))->Play();
	rainGameObject->AddComponent(new SpriteRenderer("circle.png"))->SetColor({0, 0.5, 1, 1});
	rainGameObject->SetScale(0.2, 0.2);

	rainGameObject->SetPositionX(500);

	GameObject* fireGameObject = CreateGameObject();
	fireGameObject->AddComponent(new AudioSource("fireplace.wav", true, true, 1, 1, 1))->Play();
	fireGameObject->AddComponent(new SpriteRenderer("circle.png"))->SetColor({ 1, 0.5, 0, 1 });
	fireGameObject->SetScale(0.2, 0.2);

	fireGameObject->SetPositionX(-500);
}

void AudioTestScene2::onTerminate()
{
}

void AudioTestScene2::update(double deltaTime)
{
	if (Camera::GetActiveCamera()->GetGameObject() != nullptr)
	{
		Drawer::GetInstance()->DrawCross(Camera::GetActiveCamera()->CameraToWorldPosition(glm::vec2()), 10000, false);
		Drawer::GetInstance()->DrawCircle(Camera::GetActiveCamera()->CameraToWorldPosition(glm::vec2()), 256 * Camera::GetActiveCamera()->GetZoom(), { 0,1,0,1 }, 4);
		Drawer::GetInstance()->DrawCircle(Camera::GetActiveCamera()->CameraToWorldPosition(glm::vec2()), 512 * Camera::GetActiveCamera()->GetZoom(), { 0,1,0,1 }, 4);

		Drawer::GetInstance()->DrawLine((vec2)Camera::GetActiveCamera()->GetGameObject()->GetPosition() + vec2(-256, -1000) * Camera::GetActiveCamera()->GetZoom(), (vec2)Camera::GetActiveCamera()->GetGameObject()->GetPosition() + vec2(-256, 1000) * Camera::GetActiveCamera()->GetZoom(), { 1, 0.1, 0, 1 }, 4);
		Drawer::GetInstance()->DrawLine((vec2)Camera::GetActiveCamera()->GetGameObject()->GetPosition() + vec2(256, 1000) * Camera::GetActiveCamera()->GetZoom(), (vec2)Camera::GetActiveCamera()->GetGameObject()->GetPosition() + vec2(256, -1000) * Camera::GetActiveCamera()->GetZoom(), { 1, 0.1, 0, 1 }, 4);

		Drawer::GetInstance()->DrawLine((vec2)Camera::GetActiveCamera()->GetGameObject()->GetPosition() + vec2(-512, -1000) * Camera::GetActiveCamera()->GetZoom(), (vec2)Camera::GetActiveCamera()->GetGameObject()->GetPosition() + vec2(-512, 1000) * Camera::GetActiveCamera()->GetZoom(), { 1, 0.1, 0, 1 }, 4);
		Drawer::GetInstance()->DrawLine((vec2)Camera::GetActiveCamera()->GetGameObject()->GetPosition() + vec2(512, 1000) * Camera::GetActiveCamera()->GetZoom(), (vec2)Camera::GetActiveCamera()->GetGameObject()->GetPosition() + vec2(512, -1000) * Camera::GetActiveCamera()->GetZoom(), { 1, 0.1, 0, 1 }, 4);
	}
}

void AudioTestScene2::fixedUpdate(float fixedTimestep)
{
}

void AudioTestScene2::onButtonDown(int button)
{
	if (button == INPUT_KEY_ESCAPE)
	{
		TerminateGame();
	}
	if (button == INPUT_KEY_R)
	{
		ChangeToScene("AudioTestScene2");
	}

	if (button == INPUT_KEY_BACKSPACE)
	{
		ChangeToScene("AudioTestScene");
	}

	if (button == INPUT_KEY_ENTER)
	{
		ChangeToScene("TitleScenePhysics");
	}
}

void AudioTestScene2::onMouseButtonDown(int button, glm::vec2 screenPosition, glm::vec2 worldPosition)
{
	/*glm::vec2 position = worldPosition;
	position = Camera::GetActiveCamera()->WorldToCameraPosition(position);
	position = Camera::GetActiveCamera()->CameraToWorldPosition(position);
	position = Camera::GetActiveCamera()->WorldToScreenPosition(position);
	position = Camera::GetActiveCamera()->ScreenToCameraPosition(position);
	position = Camera::GetActiveCamera()->CameraToScreenPosition(position);
	position = Camera::GetActiveCamera()->ScreenToWorldPosition(position);

	GameObject* gameObject = CreateGameObject();
	gameObject->SetPosition(position);
	gameObject->SetScale(0.1, 0.1);
	gameObject->AddComponent(new SpriteRenderer("circle.png"))->SetColor({ 0, 1, 0.2, 1 });


	glm::vec2 cameraPosition = Camera::GetActiveCamera()->WorldToCameraPosition(worldPosition);

	printf("\n\nWORLD POSITION: (%g, %g)", worldPosition.x, worldPosition.y);
	printf("\nCAMERA POSITION: (%g, %g)", cameraPosition.x, cameraPosition.y);
	printf("\nSCREEN POSITION: (%g, %g)", screenPosition.x, screenPosition.y);*/
}


