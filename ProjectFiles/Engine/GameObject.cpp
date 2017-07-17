#include "GameObject.h"

#include "Component.h"
#include "Scene.h"

#include "gtc\matrix_transform.hpp"

#include <algorithm>


GameObject::GameObject()
{
	_position = glm::vec3();
	_scale = glm::vec2(1, 1);
	_orientation = 0;

	updateTransform();
}

GameObject::~GameObject()
{
}


void GameObject::updateTransform()
{
	_transform = glm::translate(glm::mat4(), _position);
	_transform = glm::rotate(_transform, _orientation, glm::vec3(0, 0, -1));
	_transform = glm::scale(_transform, glm::vec3(_scale.x, _scale.y, 1));
}

void GameObject::addPendingComponents()
{
	if (_pendingComponentsToAdd.size() > 0)
	{
		std::vector<Component*> tempComponents;

		for (Component* component : _pendingComponentsToAdd)
		{
			if (std::find(_components.begin(), _components.end(), component) == _components.end())
			{
				_components.push_back(component);
				component->_gameObject = this;

				tempComponents.push_back(component);
			}
			else
			{
				printf("\nWARNING: GameObject::addPendingComponents -> GameObject already contains that component");
			}
		}

		_pendingComponentsToAdd.clear();

		for (Component* component : tempComponents)
		{
			component->OnAddedToGameObject();
			component->OnInitialize();
		}
	}
}

void GameObject::removePendingComponents()
{
	if (_pendingComponentsToRemove.size() > 0)
	{
		std::vector<Component*> tempComponents = _pendingComponentsToRemove;

		for (Component* component : _pendingComponentsToRemove)
		{
			auto it = std::find(_components.begin(), _components.end(), component);
			if (it != _components.end())
			{
				_components.erase(it);
			
				tempComponents.push_back(component);
			}
			else
			{
				printf("\nWARNING: GameObject::removePendingComponents -> GameObject does not contain that component");
			}
		}

		_pendingComponentsToRemove.clear();

		for (Component* component : tempComponents)
		{
			component->OnRemovedFromGameObject();
			component->_gameObject = nullptr;
		}
	}
}


void GameObject::RemoveComponent(Component * component)
{
	_pendingComponentsToRemove.push_back(component);
}

void GameObject::AddChildGameObject(GameObject * gameObject)
{
	if (gameObject == this)
	{
		printf("\nWARNING: GameObject::AddChildGameObject -> You can't add a GameObject to itself as child");
		return;
	}

	auto it = std::find(_childGameObjects.begin(), _childGameObjects.end(), gameObject);
	if (it == _childGameObjects.end())
	{
		_childGameObjects.push_back(gameObject);
		gameObject->_parentGameObject = this;
	}
	else
	{
		printf("\nWARNING: GameObject::AddChildGameObject -> GameObject already contains that child GameObject");
	}
}

void GameObject::RemoveChildGameObject(GameObject * gameObject)
{
	auto it = std::find(_childGameObjects.begin(), _childGameObjects.end(), gameObject);
	if (it != _childGameObjects.end())
	{
		_childGameObjects.erase(it);
		gameObject->_parentGameObject = nullptr;
	}
	else
	{
		printf("\nWARNING: GameObject::RemoveChildGameObject -> GameObject doesn't contains that child GameObject");
	}
}

void GameObject::Update(double deltaTime)
{
	addPendingComponents();
	removePendingComponents();

	for (Component* component : _components)
	{
		component->Update(deltaTime);
	}
}

void GameObject::FixedUpdate(float fixedTimestep)
{
	for (Component* component : _components)
	{
		component->FixedUpdate(fixedTimestep);
	}
}

void GameObject::OnInitialize()
{
	for (Component* component : _components)
	{
		component->OnInitialize();
	}

	_initialized = true;
}

void GameObject::OnTerminate()
{
	for (Component* component : _components)
	{
		component->OnTerminate();
		delete component;
	}

	for (Component* component : _pendingComponentsToAdd)
	{
		delete component;
	}

	_components.clear();
	_pendingComponentsToAdd.clear();
	_pendingComponentsToRemove.clear();
}

void GameObject::OnButtonDown(int button)
{
	for (Component* component : _components)
	{
		component->OnButtonDown(button);
	}
}

void GameObject::OnButtonUp(int button)
{
	for (Component* component : _components)
	{
		component->OnButtonUp(button);
	}
}

void GameObject::OnControllerButtonDown(int controller, int button)
{
	for (Component* component : _components)
	{
		component->OnControllerButtonDown(controller, button);
	}
}

void GameObject::OnControllerButtonUp(int controller, int button)
{
	for (Component* component : _components)
	{
		component->OnControllerButtonUp(controller, button);
	}
}

void GameObject::OnMouseButtonDown(int button, glm::vec2 screenPosition, glm::vec2 worldPosition)
{
	for (Component* component : _components)
	{
		component->OnMouseButtonDown(button, screenPosition, worldPosition);
	}
}

void GameObject::OnMouseButtonUp(int button, glm::vec2 screenPosition, glm::vec2 worldPosition)
{
	for (Component* component : _components)
	{
		component->OnMouseButtonUp(button, screenPosition, worldPosition);
	}
}

void GameObject::OnMouseMove(glm::vec2 screenPosition, glm::vec2 worldPosition)
{
	for (Component* component : _components)
	{
		component->OnMouseMove(screenPosition, worldPosition);
	}
}

void GameObject::OnCollisionEnter(ContactInfo contactInfo)
{
	for (Component* component : _components)
	{
		component->OnCollisionEnter(contactInfo);
	}
}

void GameObject::OnCollisionExit(ContactInfo contactInfo)
{
	for (Component* component : _components)
	{
		component->OnCollisionExit(contactInfo);
	}
}

void GameObject::OnCollision(ContactInfo contactInfo)
{
	for (Component* component : _components)
	{
		component->OnCollision(contactInfo);
	}
}

void GameObject::OnTriggerEnter(ContactInfo contactInfo)
{
	for (Component* component : _components)
	{
		component->OnTriggerEnter(contactInfo);
	}
}

void GameObject::OnTriggerExit(ContactInfo contactInfo)
{
	for (Component* component : _components)
	{
		component->OnTriggerExit(contactInfo);
	}
}

void GameObject::OnTrigger(ContactInfo contactInfo)
{
	for (Component* component : _components)
	{
		component->OnTrigger(contactInfo);
	}
}

Scene * GameObject::GetParentScene()
{
	return _parentScene;
}

glm::mat4 GameObject::GetTransform()
{
	if (_isTransformDirty)
	{
		updateTransform();
	}

	if (_parentGameObject != nullptr)
	{
		return _parentGameObject->GetTransform() * _transform;
	}

	return _transform;
}

bool GameObject::IsTransformDirty()
{
	return _isTransformDirty;
}

void GameObject::SetPosition(float x, float y)
{
	_position.x = x;
	_position.y = y;

	_isTransformDirty = true;
}

void GameObject::SetPosition(glm::vec2 position)
{
	SetPosition(position.x, position.y);
}

void GameObject::SetPositionX(float x)
{
	_position.x = x;

	_isTransformDirty = true;
}

void GameObject::SetPositionY(float y)
{
	_position.y = y;

	_isTransformDirty = true;
}

void GameObject::SetDepth(float depth)
{
	_position.z = depth;
	_isTransformDirty = true;
}

void GameObject::SetOrientation(float radians, bool normalize)
{
	if (normalize)
	{
		float pi2 = glm::pi<float>() * 2;

		if (radians > pi2)
		{
			radians -= pi2 * int(radians / pi2);
		}
		else if (radians < 0)
		{
			radians += pi2 * int(-radians / pi2) + pi2;
		}
	}

	_orientation = radians;

	_isTransformDirty = true;
}

void GameObject::SetScale(float x, float y)
{
	_scale.x = x;
	_scale.y = y;

	_isTransformDirty = true;
}

void GameObject::SetScale(glm::vec2 scale)
{
	SetScale(scale.x, scale.y);
}

void GameObject::SetScaleX(float scaleX)
{
	_scale.x = scaleX;

	_isTransformDirty = true;
}

void GameObject::SetScaleY(float scaleY)
{
	_scale.y = scaleY;

	_isTransformDirty = true;
}

glm::vec3 GameObject::GetPosition(bool local)
{
	if (local)
	{
		return _position;
	}

	return (vec3)(GetTransform() * vec4(0, 0, 0, 1));
}

float GameObject::GetDepth(bool localSpace)
{
	return GetPosition(localSpace).z;
}

float GameObject::GetOrientation()
{
	return _orientation;
}

glm::vec2 GameObject::GetScale()
{
	return _scale;
}
