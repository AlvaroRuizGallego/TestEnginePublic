#ifndef COMPONENT_H
#define COMPONENT_H

#include "GameObject.h"
#include "InputBinds.h"

#include "glm.hpp"

using namespace glm;
using namespace std;

class __declspec(dllexport) Component
{
	friend class GameObject;

	protected:
		GameObject* _gameObject = nullptr;
		bool _unique = false;

	public:
		Component();
		virtual ~Component();

		virtual void Update(double deltaTime);
		virtual void FixedUpdate(float fixedTimestep);

		virtual void OnAddedToGameObject();
		virtual void OnRemovedFromGameObject();
		virtual void OnInitialize();
		virtual void OnTerminate();

		virtual void OnButtonDown(int button);
		virtual void OnButtonUp(int button);
		virtual void OnControllerButtonDown(int controller, int button);
		virtual void OnControllerButtonUp(int controller, int button);
		virtual void OnMouseButtonDown(int button, glm::vec2 screenPosition, glm::vec2 worldPosition);
		virtual void OnMouseButtonUp(int button, glm::vec2 screenPosition, glm::vec2 worldPosition);
		virtual void OnMouseMove(glm::vec2 screenPosition, glm::vec2 worldPosition);

		virtual void OnCollisionEnter(ContactInfo contactInfo);
		virtual void OnCollisionExit(ContactInfo contactInfo);
		virtual void OnCollision(ContactInfo contactInfo);
		virtual void OnTriggerEnter(ContactInfo contactInfo);
		virtual void OnTriggerExit(ContactInfo contactInfo);
		virtual void OnTrigger(ContactInfo contactInfo);

		bool IsUnique();

		GameObject* GetGameObject();
};

#endif
