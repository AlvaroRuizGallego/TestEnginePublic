#include "ParallaxTestScene.h"

#include "SpriteSheet.h"
#include "InputBinds.h"

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "AnimatedSpriteRenderer.h"
#include "Rigidbody.h"

#include "TestStateMachine.h"

#include "Drawer.h"

#include "ControllerInput.h"

#include "SpriteSheetManager.h"


ParallaxTestScene::ParallaxTestScene()
{
}

ParallaxTestScene::~ParallaxTestScene()
{
}

void ParallaxTestScene::onInitialize()
{
	//Camera
	_cameraGameObject = CreateGameObject();
	_camera = new Camera();
	_cameraGameObject->AddComponent(_camera);


	//Spritesheet
	_spriteSheet = SpriteSheetManager::GetInstance()->GetSpriteSheet("gooniesSameSizeCentered.json");


	_gameObject = CreateGameObject();
	_gameObject->AddComponent(new AnimatedSpriteRenderer(_spriteSheet, "goonie_", 0.1f, 0, true));
	//_gameObject->SetPositionX(100);

	//_gameObject->SetScale(0.01, 0.01);
	_gameObject->SetPosition(GetReferenceWindowWidth()*0.5, GetReferenceWindowHeight() * 0.5);

	//_gameObject->GetComponent<AnimatedSpriteRenderer>()->SetSpriteSize({ 1, 1 });


	//Backgroud
	//Background
	_backgroundGameObject = CreateGameObject();
	_backgroundGameObject->AddComponent(new SpriteRenderer("background.png"));
	_backgroundGameObject->SetScale(2, 2);
	_backgroundGameObject->AddComponent(new TestStateMachine());
	_backgroundGameObject->GetComponent<SpriteRenderer>()->SetParallax(0.2);
}

void ParallaxTestScene::onTerminate()
{
}

void ParallaxTestScene::update(double deltaTime)
{
	//int cameraHDirection = (_cameraButtons.moveLeft ? -1 : 0) + (_cameraButtons.moveRight ? 1 : 0);
	//int cameraVDirection = (_cameraButtons.moveDown ? -1 : 0) + (_cameraButtons.moveUp ? 1 : 0);
	//int cameraRotationDirection = (_cameraButtons.rotateCCW ? 1 : 0) + (_cameraButtons.rotateCW ? -1 : 0);
	//int cameraZoomDirection = (_cameraButtons.zoomOut ? -1 : 0) + (_cameraButtons.zoomIn ? 1 : 0);

	//_cameraGameObject->SetPosition(_cameraGameObject->GetPosition().x + cameraHDirection * _cameraMovementSpeed * deltaTime, _cameraGameObject->GetPosition().y + cameraVDirection * _cameraMovementSpeed * deltaTime);
	//_cameraGameObject->SetOrientation(_cameraGameObject->GetOrientation() + cameraRotationDirection* _cameraRotationSpeed * deltaTime);
	//_camera->SetZoom(_camera->GetZoom() + cameraZoomDirection * _cameraZoomSpeed * _camera->GetZoom() * deltaTime);

	ControllerInput* controllerInput = ControllerInput::GetInstance();

	_cameraGameObject->SetPosition(_cameraGameObject->GetPosition().x + controllerInput->GetAxis(CONTROLLER_1, INPUT_CONTROLLER_AXIS_1) * _cameraMovementSpeed * deltaTime, _cameraGameObject->GetPosition().y + controllerInput->GetAxis(CONTROLLER_1, INPUT_CONTROLLER_AXIS_2) * _cameraMovementSpeed * deltaTime);
	_cameraGameObject->SetOrientation(_cameraGameObject->GetOrientation() + controllerInput->GetAxis(CONTROLLER_2, INPUT_CONTROLLER_AXIS_3)* _cameraRotationSpeed * deltaTime);
	_camera->SetZoom(_camera->GetZoom() + (glm::clamp(controllerInput->GetAxis(CONTROLLER_2, INPUT_CONTROLLER_AXIS_6) + 1.0f, 0.0f, 1.0f) - glm::clamp(controllerInput->GetAxis(CONTROLLER_1, INPUT_CONTROLLER_AXIS_5) + 1.0f, 0.0f, 1.0f)) * _cameraZoomSpeed * _camera->GetZoom() * deltaTime);


	//printf("\nCONTROLLER 1 -> AXIS 1: %g", controllerInput->GetAxis(CONTROLLER_1, INPUT_CONTROLLER_AXIS_1));//LEFT H
	//printf("\nCONTROLLER 1 -> AXIS 2: %g", controllerInput->GetAxis(CONTROLLER_1, INPUT_CONTROLLER_AXIS_2));//LEFT V
	//printf("\nCONTROLLER 1 -> AXIS 3: %g", controllerInput->GetAxis(CONTROLLER_1, INPUT_CONTROLLER_AXIS_3));//RIGHT H
	//printf("\nCONTROLLER 1 -> AXIS 4: %g", controllerInput->GetAxis(CONTROLLER_1, INPUT_CONTROLLER_AXIS_4));//RIGHT V
	//printf("\nCONTROLLER 1 -> AXIS 5: %g", controllerInput->GetAxis(CONTROLLER_1, INPUT_CONTROLLER_AXIS_5));//LEFT TRIGGER
	//printf("\nCONTROLLER 1 -> AXIS 6: %g", controllerInput->GetAxis(CONTROLLER_1, INPUT_CONTROLLER_AXIS_6));//RIGHT TRIGGER

	Drawer::GetInstance()->DrawCross(vec2(0, 0), 100000);
	Drawer::GetInstance()->DrawCross(vec2(100, 0), 100000);
	Drawer::GetInstance()->DrawCross(vec2(200, 0), 100000);
	Drawer::GetInstance()->DrawCross(vec2(300, 0), 100000);
	Drawer::GetInstance()->DrawCross(vec2(400, 0), 100000);
	Drawer::GetInstance()->DrawCross(vec2(500, 0), 100000);


	//glm::vec2 minScreenView, maxScreenView;
	//minScreenView = Camera::WorldToScreenPosition()

	Rectangle screenRegion = Camera::GetActiveCamera()->GetScreenRegion();
	Drawer::GetInstance()->DrawCross(Camera::GetActiveCamera()->ScreenToWorldPosition(screenRegion.min), 100000);
	Drawer::GetInstance()->DrawCross(Camera::GetActiveCamera()->ScreenToWorldPosition(screenRegion.max), 100000);
}

void ParallaxTestScene::fixedUpdate(float fixedTimestep)
{
}


void ParallaxTestScene::onButtonDown(int button)
{
	printf("\n(%i) DOWN\n", button);

	if (button == INPUT_KEY_ESCAPE)
	{
		TerminateGame();
	}


	if (button == INPUT_KEY_RIGHT || button == INPUT_CONTROLLER_BUTTON_1)
	{
		_cameraButtons.moveRight = true;
	}
	if (button == INPUT_KEY_LEFT)
	{
		_cameraButtons.moveLeft = true;
	}
	if (button == INPUT_KEY_UP)
	{
		_cameraButtons.moveUp = true;
	}
	if (button == INPUT_KEY_DOWN)
	{
		_cameraButtons.moveDown = true;
	}
	if (button == INPUT_KEY_D)
	{
		_cameraButtons.rotateCW = true;
	}
	if (button == INPUT_KEY_A)
	{
		_cameraButtons.rotateCCW = true;
	}
	if (button == INPUT_KEY_W)
	{
		_cameraButtons.zoomIn = true;
	}
	if (button == INPUT_KEY_S)
	{
		_cameraButtons.zoomOut = true;
	}
	if (button == INPUT_KEY_ENTER)
	{
		ChangeToScene("MemoryManagementScene2");
	}
}

void ParallaxTestScene::onButtonUp(int button)
{
	printf("\n(%i) UP\n", button);

	if (button == INPUT_KEY_RIGHT ||button == INPUT_CONTROLLER_BUTTON_1)
	{
		_cameraButtons.moveRight = false;
	}
	if (button == INPUT_KEY_LEFT)
	{
		_cameraButtons.moveLeft = false;
	}
	if (button == INPUT_KEY_UP)
	{
		_cameraButtons.moveUp = false;
	}
	if (button == INPUT_KEY_DOWN)
	{
		_cameraButtons.moveDown = false;
	}
	if (button == INPUT_KEY_D)
	{
		_cameraButtons.rotateCW = false;
	}
	if (button == INPUT_KEY_A)
	{
		_cameraButtons.rotateCCW = false;
	}
	if (button == INPUT_KEY_W)
	{
		_cameraButtons.zoomIn = false;
	}
	if (button == INPUT_KEY_S)
	{
		_cameraButtons.zoomOut = false;
	}
}

void ParallaxTestScene::onControllerButtonDown(int controller, int button)
{
	if (button == INPUT_CONTROLLER_BUTTON_1)
	{
		_cameraButtons.moveRight = true;
	}
}

void ParallaxTestScene::onControllerButtonUp(int controller, int button)
{
	if (button == INPUT_CONTROLLER_BUTTON_1)
	{
		_cameraButtons.moveRight = false;
	}
}

void ParallaxTestScene::onMouseButtonDown(int button, glm::vec2 position, glm::vec2 worldPosition)
{
	if (button == INPUT_MOUSE_BUTTON_LEFT)
	{
		printf("\nMOUSE POSITION: (%g, %g)", position.x, position.y);
		printf("\nMOUSE WORLD POSITION: (%g, %g)", worldPosition.x, worldPosition.y);


		glm::vec2 cameraPosition1 = Camera::GetActiveCamera()->WorldToCameraPosition(worldPosition);
		glm::vec2 worldPosition2 = Camera::GetActiveCamera()->CameraToWorldPosition(Camera::GetActiveCamera()->WorldToCameraPosition(worldPosition));

		printf("\nCAMERA POSITION 1: (%g, %g)", cameraPosition1.x, cameraPosition1.y);
		printf("\nWORLD POSITION: (%g, %g)", worldPosition2.x, worldPosition2.y);
	}
	else if (button == INPUT_MOUSE_BUTTON_RIGHT)
	{
	}
	else if (button == INPUT_MOUSE_BUTTON_MIDDLE)
	{
	}
}

void ParallaxTestScene::onMouseButtonUp(int button, glm::vec2 position, glm::vec2 worldPosition)
{
	if (button == INPUT_MOUSE_BUTTON_LEFT)
	{

	}
	else if (button == INPUT_MOUSE_BUTTON_RIGHT)
	{

	}
	else if (button == INPUT_MOUSE_BUTTON_MIDDLE)
	{

	}
}

void ParallaxTestScene::onMouseMove(glm::vec2 position, glm::vec2 worldPosition)
{
}

