#include "Scene.h"

#include "_SpriteManager.h"
#include "GameObject.h"

#include <algorithm>



Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::addPendingGameObjects()
{
	if (_pendingGameObjectsToAdd.size() > 0)
	{
		for (GameObject * gameObject : _pendingGameObjectsToAdd)
		{
			_gameObjects.push_back(gameObject);
			gameObject->OnInitialize();
		}

		_pendingGameObjectsToAdd.clear();
	}
}

void Scene::destroyPendingGameObjects()
{
	if (_pendingGameObjectsToDestroy.size() > 0)
	{
		for (GameObject * gameObject : _pendingGameObjectsToDestroy)
		{
			_gameObjects.erase(std::find(_gameObjects.begin(), _gameObjects.end(), gameObject));
			gameObject->OnTerminate();

			delete gameObject;
		}

		_pendingGameObjectsToDestroy.clear();
	}
}

void Scene::onInitialize()
{
}

void Scene::onTerminate()
{
}

void Scene::update(double deltaTime)
{
}

void Scene::fixedUpdate(float fixedTimestep)
{
}

void Scene::onButtonDown(int button)
{
}

void Scene::onButtonUp(int button)
{
}

void Scene::onControllerButtonDown(int controller, int button)
{
}

void Scene::onControllerButtonUp(int controller, int button)
{
}

void Scene::onMouseButtonDown(int button, glm::vec2 screenPosition, glm::vec2 worldPosition)
{
}

void Scene::onMouseButtonUp(int button, glm::vec2 screenPosition, glm::vec2 worldPosition)
{
}

void Scene::onMouseMove(glm::vec2 screenPosition, glm::vec2 worldPosition)
{
}

void Scene::Initialize()
{
	onInitialize();
}

void Scene::Terminate()
{
	onTerminate();

	for (GameObject* gameObject : _gameObjects)
	{
		gameObject->OnTerminate();
		delete gameObject;
	}

	for (GameObject* gameObject : _pendingGameObjectsToAdd)
	{
		delete gameObject;
	}

	_gameObjects.clear();
	_pendingGameObjectsToAdd.clear();
	_pendingGameObjectsToDestroy.clear();
}

void Scene::Update(double deltaTime)
{
	addPendingGameObjects();
	destroyPendingGameObjects();

	update(deltaTime);

	for (GameObject* gameObject : _gameObjects)
	{
		gameObject->Update(deltaTime);
	}
}

void Scene::FixedUpdate(float fixedTimestep)
{
	fixedUpdate(fixedTimestep);

	for (GameObject* gameObject : _gameObjects)
	{
		gameObject->FixedUpdate(fixedTimestep);
	}
}


void Scene::OnButtonDown(int button)
{
	onButtonDown(button);

	for (GameObject* gameObject : _gameObjects)
	{
		gameObject->OnButtonDown(button);
	}
}

void Scene::OnButtonUp(int button)
{
	onButtonUp(button);

	for (GameObject* gameObject : _gameObjects)
	{
		gameObject->OnButtonUp(button);
	}
}

void Scene::OnControllerButtonDown(int controller, int button)
{
	onControllerButtonDown(controller, button);

	for (GameObject* gameObject : _gameObjects)
	{
		gameObject->OnControllerButtonDown(controller, button);
	}
}

void Scene::OnControllerButtonUp(int controller, int button)
{
	onControllerButtonUp(controller, button);

	for (GameObject* gameObject : _gameObjects)
	{
		gameObject->OnControllerButtonUp(controller, button);
	}
}

void Scene::OnMouseButtonDown(int button, glm::vec2 screenPosition, glm::vec2 worldPosition)
{
	onMouseButtonDown(button, screenPosition, worldPosition);

	for (GameObject* gameObject : _gameObjects)
	{
		gameObject->OnMouseButtonDown(button, screenPosition, worldPosition);
	}
}

void Scene::OnMouseButtonUp(int button, glm::vec2 screenPosition, glm::vec2 worldPosition)
{
	onMouseButtonUp(button, screenPosition, worldPosition);

	for (GameObject* gameObject : _gameObjects)
	{
		gameObject->OnMouseButtonUp(button, screenPosition, worldPosition);
	}
}

void Scene::OnMouseMove(glm::vec2 screenPosition, glm::vec2 worldPosition)
{
	onMouseMove(screenPosition, worldPosition);

	for (GameObject* gameObject : _gameObjects)
	{
		gameObject->OnMouseMove(screenPosition, worldPosition);
	}
}

GameObject* Scene::CreateGameObject()
{
	GameObject* gameObject = new GameObject();
	gameObject->_parentScene = this;

	_pendingGameObjectsToAdd.push_back(gameObject);

	return gameObject;
}

void Scene::DestroyGameObject(GameObject * gameObject)
{
	if (gameObject->_parentScene == this)
	{
		_pendingGameObjectsToDestroy.push_back(gameObject);
	}
}

void Scene::ChangeToScene(std::string  name)
{
	Game::GetInstance()->ChangeToScene(name);
}

void Scene::TerminateGame()
{
	Game::GetInstance()->Terminate();
}

int Scene::GetWindowWidth()
{
	return Game::GetInstance()->getWindowWidth();
}

int Scene::GetWindowHeight()
{
	return Game::GetInstance()->getWindowHeight();
}

int Scene::GetReferenceWindowWidth()
{
	return Game::GetInstance()->getReferenceWindowWidth();
}

int Scene::GetReferenceWindowHeight()
{
	return Game::GetInstance()->getReferenceWindowHeight();
}

