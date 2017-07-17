#include "_PhysicsEngine.h"

#include "Drawer.h"
#include "Rigidbody.h"
#include "CollisionQuerier.h"

#include <algorithm>


_PhysicsEngine* _PhysicsEngine::_instance = nullptr;

_PhysicsEngine::_PhysicsEngine()
{
	_instance = nullptr;
}

void _PhysicsEngine::updateArbiters()
{
	for (int i = 0; i < _colliders.size(); ++i)
	{
		for (int j = i + 1; j < _colliders.size(); ++j)
		{
			Collider* colliderA = _colliders[i];
			Collider* colliderB = _colliders[j];

			if (colliderA->GetRigidbody() == colliderB->GetRigidbody() && colliderA->GetRigidbody() != nullptr) continue;

			std::vector<Contact> contacts;

			_ArbiterKey arbiterKey = _ArbiterKey(colliderA, colliderB);

			auto it = _arbiters.find(arbiterKey);

			if (CollisionQuerier::GetInstance()->CheckCollision(colliderA, colliderB, contacts))
			{
				createContactEvent(colliderA, colliderB, contacts);

				if (it == _arbiters.end())
				{
					createContactEnterEvent(colliderA, colliderB, contacts);
					_arbiters.emplace(arbiterKey, _Arbiter(colliderA, colliderB, contacts));
				}
				else
				{
					it->second.UpdateContacts(contacts);
				}
			}
			else
			{
				if (it != _arbiters.end())
				{
					createContactExitEvent(colliderA, colliderB);
					_arbiters.erase(arbiterKey);
				}
			}
		}
	}
}

void _PhysicsEngine::createContactEnterEvent(Collider* colliderA, Collider* colliderB, std::vector<Contact> contacts)
{
	std::vector<Contact> inverseNormalContacts = contacts;

	for (int i = 0; i < inverseNormalContacts.size(); ++i)
	{
		inverseNormalContacts[i].normal *= -1;
	}

	if (!colliderA->IsTrigger() && !colliderB->IsTrigger())
	{
		_collisionEnterEvents.push_back(std::pair<Collider*, ContactInfo>(colliderA, { contacts, colliderA, colliderB }));
		_collisionEnterEvents.push_back(std::pair<Collider*, ContactInfo>(colliderB, { inverseNormalContacts, colliderB, colliderA }));
		
	}
	else
	{
		_triggerEnterEvents.push_back(std::pair<Collider*, ContactInfo>(colliderA, { contacts, colliderA, colliderB }));
		_triggerEnterEvents.push_back(std::pair<Collider*, ContactInfo>(colliderB, { inverseNormalContacts, colliderB, colliderA }));
	}
}

void _PhysicsEngine::createContactExitEvent(Collider * colliderA, Collider * colliderB)
{
	if (!colliderA->IsTrigger() && !colliderB->IsTrigger())
	{
		_collisionExitEvents.push_back(std::pair<Collider*, ContactInfo>(colliderA, { std::vector<Contact>(), colliderA, colliderB }));
		_collisionExitEvents.push_back(std::pair<Collider*, ContactInfo>(colliderB, { std::vector<Contact>(), colliderB, colliderA }));

	}
	else
	{
		_triggerExitEvents.push_back(std::pair<Collider*, ContactInfo>(colliderA, { std::vector<Contact>(), colliderA, colliderB }));
		_triggerExitEvents.push_back(std::pair<Collider*, ContactInfo>(colliderB, { std::vector<Contact>(), colliderB, colliderA }));
	}
}

void _PhysicsEngine::createContactEvent(Collider * colliderA, Collider * colliderB, std::vector<Contact> contacts)
{
	std::vector<Contact> inverseNormalContacts = contacts;

	for (int i = 0; i < inverseNormalContacts.size(); ++i)
	{
		inverseNormalContacts[i].normal *= -1;
	}

	if (!colliderA->IsTrigger() && !colliderB->IsTrigger())
	{
		_collisionEvents.push_back(std::pair<Collider*, ContactInfo>(colliderA, { contacts, colliderA, colliderB }));
		_collisionEvents.push_back(std::pair<Collider*, ContactInfo>(colliderB, { inverseNormalContacts, colliderB, colliderA }));

	}
	else
	{
		_triggerEvents.push_back(std::pair<Collider*, ContactInfo>(colliderA, { contacts, colliderA, colliderB }));
		_triggerEvents.push_back(std::pair<Collider*, ContactInfo>(colliderB, { inverseNormalContacts, colliderB, colliderA }));
	}
}

void _PhysicsEngine::invokeContactEvents()
{
	for (std::pair<Collider*, ContactInfo> collisionEvent : _collisionEnterEvents)
	{
		collisionEvent.first->GetGameObject()->OnCollisionEnter(collisionEvent.second);
	}
	
	for (std::pair<Collider*, ContactInfo> collisionEvent : _collisionExitEvents)
	{
		collisionEvent.first->GetGameObject()->OnCollisionExit(collisionEvent.second);
	}

	for (std::pair<Collider*, ContactInfo> collisionEvent : _collisionEvents)
	{
		collisionEvent.first->GetGameObject()->OnCollision(collisionEvent.second);
	}

	for (std::pair<Collider*, ContactInfo> triggerEvent : _triggerEnterEvents)
	{
		triggerEvent.first->GetGameObject()->OnTriggerEnter(triggerEvent.second);
	}

	for (std::pair<Collider*, ContactInfo> triggerEvent : _triggerExitEvents)
	{
		triggerEvent.first->GetGameObject()->OnTriggerExit(triggerEvent.second);
	}

	for (std::pair<Collider*, ContactInfo> triggerEvent : _triggerEvents)
	{
		triggerEvent.first->GetGameObject()->OnTrigger(triggerEvent.second);
	}

	_collisionEnterEvents.clear();
	_collisionExitEvents.clear();
	_collisionEvents.clear();
	_triggerEnterEvents.clear();
	_triggerExitEvents.clear();
	_triggerEvents.clear();
}

_PhysicsEngine::~_PhysicsEngine()
{
}

_PhysicsEngine * _PhysicsEngine::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new _PhysicsEngine();
	}

	return _instance;
}

void _PhysicsEngine::AddCollider(Collider * collider)
{
	_colliders.push_back(collider);
}

void _PhysicsEngine::RemoveCollider(Collider * collider)
{
	_colliders.erase(std::remove(_colliders.begin(), _colliders.end(), collider), _colliders.end());
}

void _PhysicsEngine::FixedUpdate(float fixedTimestep)
{
	//Update arbiters
	updateArbiters();

	//Integrate forces
	for (int i = 0; i < _colliders.size(); ++i)
	{
		Rigidbody* rigidbody = _colliders[i]->GetRigidbody();
		if (rigidbody != nullptr) rigidbody->integrateForces(fixedTimestep);
	}

	//Perform pre-steps
	for (auto it = _arbiters.begin(); it != _arbiters.end(); ++it)
	{
		it->second.PreStep(fixedTimestep);
	}

	//Perform iterations
	for (int i = 0; i < _collisionResolutionSteps; ++i)
	{
		for (auto it = _arbiters.begin(); it != _arbiters.end(); ++it)
		{
			it->second.Step();
		}
	}

	//Perform post-step
	for (auto it = _arbiters.begin(); it != _arbiters.end(); ++it)
	{
		it->second.PostStep();
	}

	//Integrate velocities
	for (int i = 0; i < _colliders.size(); ++i)
	{
		Rigidbody* rigidbody = _colliders[i]->GetRigidbody();
		if (rigidbody != nullptr) rigidbody->applyVelocities(fixedTimestep);
	}

	//Events
	invokeContactEvents();
}

void _PhysicsEngine::Clear()
{
	_colliders.clear();
	_arbiters.clear();
	_collisionEnterEvents.clear();
	_collisionExitEvents.clear();
	_collisionEvents.clear();
	_triggerEnterEvents.clear();
	_triggerExitEvents.clear();
	_triggerEvents.clear();
}

void _PhysicsEngine::SetParameters(vec2 gravity, int collisionResolutionSteps, float allowedPenetration, float penetrationBiasFactor, float minRigidbodySpeed)
{
	_gravity = gravity;
	_collisionResolutionSteps = collisionResolutionSteps;
	_allowedPenetration = allowedPenetration;
	_penetrationBiasFactor = penetrationBiasFactor;
	_minRigidbodySpeed = minRigidbodySpeed;
}
