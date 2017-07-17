#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "CollisionData.h"

#include "glm.hpp"

#include <map>
#include <typeinfo>

class Scene;
class Component;


class __declspec(dllexport) GameObject
{
	friend class Scene;

	private:
		Scene* _parentScene = nullptr;

		GameObject* _parentGameObject = nullptr;
		std::vector<GameObject*> _childGameObjects;

		std::vector<Component*> _components;

		glm::vec3 _position;
		glm::vec2 _scale;
		float _orientation;//Clockwise //Radians
		glm::mat4 _transform;
		bool _isTransformDirty;

		bool _initialized = false;


		std::vector<Component*> _pendingComponentsToAdd;
		std::vector<Component*> _pendingComponentsToRemove;


		void updateTransform();
		void addPendingComponents();
		void removePendingComponents();

		template<typename T> bool canAddComponent(Component* component)
		{
			if(component->IsUnique() && GetComponent<T>() != nullptr)
			{
				printf("\nWARNING: GameObject::AddComponent -> Can't add more that one Component of type: %s", typeid(T).name());
				return false;
			}
			else
			{
				return true;
			}
		}

		GameObject();
		~GameObject();


	public:
		std::string tag = "";

		void RemoveComponent(Component* component);

		void AddChildGameObject(GameObject* gameObject);
		void RemoveChildGameObject(GameObject* gameObject);

		void Update(double deltaTime);
		void FixedUpdate(float fixedTimestep);

		void OnInitialize();
		void OnTerminate();

		void OnButtonDown(int button);
		void OnButtonUp(int button);
		void OnControllerButtonDown(int controller, int button);
		void OnControllerButtonUp(int controller, int button);
		void OnMouseButtonDown(int button, glm::vec2 screenPosition, glm::vec2 worldPosition);
		void OnMouseButtonUp(int button, glm::vec2 screenPosition, glm::vec2 worldPosition);
		void OnMouseMove(glm::vec2 screenPosition, glm::vec2 worldPosition);

		void OnCollisionEnter(ContactInfo contactInfo);
		void OnCollisionExit(ContactInfo contactInfo);
		void OnCollision(ContactInfo contactInfo);
		void OnTriggerEnter(ContactInfo contactInfo);
		void OnTriggerExit(ContactInfo contactInfo);
		void OnTrigger(ContactInfo contactInfo);

		Scene* GetParentScene();

		template<typename T> T* AddComponent(T* component)
		{
			if (canAddComponent<T>(component))
			{
				_pendingComponentsToAdd.push_back(component);
			}

			return component;
		}

		template<typename T> T* GetComponent()
		{
			T* ptr;
			for (Component* component : _components)
			{
				ptr = dynamic_cast<T*>(component);
				if (ptr != nullptr)
				{
					return ptr;
				}
			}
			for (Component* component : _pendingComponentsToAdd)
			{
				ptr = dynamic_cast<T*>(component);
				if (ptr != nullptr)
				{
					return ptr;
				}
			}

			return nullptr;
		}

		template<typename T> T* GetComponentInChildren()
		{
			Component* component;

			for(GameObject* gameObject : _childGameObjects)
			{
				component = gameObject->GetComponent<T>();

				if (component != nullptr)
				{
					return component;
				}
			}

			return nullptr;
		}

		template<typename T> std::vector<T*> GetComponents()
		{
			std::vector<T*> components;
			T* ptr;
			for (Component* component : _components)
			{
				ptr = dynamic_cast<T*>(component);
				if (ptr != nullptr)
				{
					components.push_back(ptr);
				}
			}

			for (Component* component : _pendingComponentsToAdd)
			{
				ptr = dynamic_cast<T*>(component);
				if (ptr != nullptr)
				{
					components.push_back(ptr);
				}
			}

			return components;
		}

		template<typename T> std::vector<T*> GetComponentsInChildren()
		{
			std::vector<T*> allComponents;
			std::vector<T*> components;

			for(GameObject* gameObject : _childGameObjects)
			{
				components = gameObject->GetComponents<T>();

				for (int i = 0; i < components.size(); i++)
				{
					allComponents.push_back(components[i]);
				}
			}

			return allComponents;
		}

		
		void SetPosition(float x, float y);
		void SetPosition(glm::vec2 position);
		void SetPositionX(float x);
		void SetPositionY(float y);
		void SetDepth(float depth);

		void SetOrientation(float radians, bool normalize = true);

		void SetScale(float x, float y);
		void SetScale(glm::vec2 scale);
		void SetScaleX(float scaleX);
		void SetScaleY(float scaleY);

		glm::vec3 GetPosition(bool localSpace = true);
		float GetDepth(bool localSpace = true);
		float GetOrientation();
		glm::vec2 GetScale();
		
		glm::mat4 GetTransform();

		bool IsTransformDirty();
};

#endif
