#ifndef SCENE_H
#define SCENE_H

#include "Game.h"
#include "Camera.h"
#include "InputBinds.h"
#include "SceneManager.h"

#include "glm.hpp"

#include <vector>

class GameObject;

using namespace glm;
using namespace std;

class __declspec(dllexport) Scene
{
	private:

		std::vector<GameObject*> _gameObjects;
		std::vector<GameObject*> _pendingGameObjectsToAdd;
		std::vector<GameObject*> _pendingGameObjectsToDestroy;


		void addPendingGameObjects();
		void destroyPendingGameObjects();

	protected:
		virtual ~Scene();

		virtual void onInitialize();
		virtual void onTerminate();

		virtual void update(double deltaTime);
		virtual void fixedUpdate(float fixedTimestep);

		virtual void onButtonDown(int button);
		virtual void onButtonUp(int button);
		virtual void onControllerButtonDown(int controller, int button);
		virtual void onControllerButtonUp(int controller, int button);
		virtual void onMouseButtonDown(int button, glm::vec2 screenPosition, glm::vec2 worldPosition);
		virtual void onMouseButtonUp(int button, glm::vec2 screenPosition, glm::vec2 worldPosition);
		virtual void onMouseMove(glm::vec2 screenPosition, glm::vec2 worldPosition);

	public:
		Scene();

		void Initialize();
		void Terminate();

		void Update(double deltaTime);
		void FixedUpdate(float fixedTimestep);

		void OnButtonDown(int button);
		void OnButtonUp(int button);
		void OnControllerButtonDown(int controller, int button);
		void OnControllerButtonUp(int controller, int button);
		void OnMouseButtonDown(int button, glm::vec2 screenPosition, glm::vec2 worldPosition);
		void OnMouseButtonUp(int button, glm::vec2 screenPosition, glm::vec2 worldPosition);
		void OnMouseMove(glm::vec2 screenPosition, glm::vec2 worldPosition);


		GameObject* CreateGameObject();
		void DestroyGameObject(GameObject* gameObject);

		void ChangeToScene(std::string  name);
		void TerminateGame();

		int GetWindowWidth();
		int GetWindowHeight();
		int GetReferenceWindowWidth();
		int GetReferenceWindowHeight();

		template<typename T> T* FindComponentInScene()
		{
			for (GameObject* gameObject : _gameObjects)
			{
				T* component = gameObject->GetComponent<T>();

				if (component != nullptr) return component;
			}
			
			for (GameObject* gameObject : _pendingGameObjectsToAdd)
			{
				T* component = gameObject->GetComponent<T>();

				if (component != nullptr) return component;
			}
			return nullptr;
		}

		template<typename T> std::vector<T*> FindComponentsInScene()
		{
			std::vector<T*> components;

			for (GameObject* gameObject : _gameObjects)
			{
				std::vector<T*> gameObjectComponents = gameObject->GetComponents<T>();
				components.insert(components.end(), gameObjectComponents.begin(), gameObjectComponents.end());
			}

			for (GameObject* gameObject : _pendingGameObjectsToAdd)
			{
				std::vector<T*> gameObjectComponents = gameObject->GetComponents<T>();
				components.insert(components.end(), gameObjectComponents.begin(), gameObjectComponents.end());
			}

			return components;
		}
};

#endif

