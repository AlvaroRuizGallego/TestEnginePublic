#include "Component.h"
#include "Component.h"
#include "Component.h"


Component::Component()
{
}

Component::~Component()
{
}

void Component::OnInitialize()
{
}

void Component::OnTerminate()
{
}

void Component::Update(double deltaTime)
{
}

void Component::FixedUpdate(float fixedTimestep)
{
}

void Component::OnAddedToGameObject()
{
}

void Component::OnRemovedFromGameObject()
{
}

void Component::OnButtonDown(int button)
{
}

void Component::OnButtonUp(int button)
{
}

void Component::OnControllerButtonDown(int controller, int button)
{
}

void Component::OnControllerButtonUp(int controller, int button)
{
}

void Component::OnMouseButtonDown(int button, glm::vec2 screenPosition, glm::vec2 worldPosition)
{
}

void Component::OnMouseButtonUp(int button, glm::vec2 screenPosition, glm::vec2 worldPosition)
{
}

void Component::OnMouseMove(glm::vec2 screenPosition, glm::vec2 worldPosition)
{
}

void Component::OnCollisionEnter(ContactInfo contactInfo)
{
}

void Component::OnCollisionExit(ContactInfo contactInfo)
{
}

void Component::OnCollision(ContactInfo contactInfo)
{
}

void Component::OnTriggerEnter(ContactInfo contactInfo)
{
}

void Component::OnTriggerExit(ContactInfo contactInfo)
{
}

void Component::OnTrigger(ContactInfo contactInfo)
{
}

bool Component::IsUnique()
{
	return _unique;
}

GameObject * Component::GetGameObject()
{
	return _gameObject;
}
