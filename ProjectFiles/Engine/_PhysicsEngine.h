#ifndef _PHYSICS_ENGINE_H
#define _PHYSICS_ENGINE_H

#include "Collider.h"
#include "CollisionData.h"
#include "_Arbiter.h"

#include <vector>
#include <map>



class _PhysicsEngine
{
	friend class CollisionQuerier;
	friend class Rigidbody;
	friend class _Arbiter;

	private:
		static _PhysicsEngine* _instance;

		std::vector<Collider*> _colliders;
		std::map<_ArbiterKey, _Arbiter> _arbiters;

		std::vector<std::pair<Collider*, ContactInfo>> _collisionEnterEvents;
		std::vector<std::pair<Collider*, ContactInfo>> _triggerEnterEvents;
		std::vector<std::pair<Collider*, ContactInfo>> _collisionExitEvents;
		std::vector<std::pair<Collider*, ContactInfo>> _triggerExitEvents;
		std::vector<std::pair<Collider*, ContactInfo>> _collisionEvents;
		std::vector<std::pair<Collider*, ContactInfo>> _triggerEvents;

		int _collisionResolutionSteps;
		float _allowedPenetration;
		float _penetrationBiasFactor;
		float _minRigidbodySpeed;
		vec2 _gravity;


		_PhysicsEngine();

		void updateArbiters();
		void createContactEnterEvent(Collider* colliderA, Collider* colliderB, std::vector<Contact> contacts);
		void createContactExitEvent(Collider* colliderA, Collider* colliderB);
		void createContactEvent(Collider* colliderA, Collider* colliderB, std::vector<Contact> contacts);

		void invokeContactEvents();

	public:
		~_PhysicsEngine();

		static _PhysicsEngine* GetInstance();

		void AddCollider(Collider* collider);
		void RemoveCollider(Collider* collider);

		void FixedUpdate(float fixedTimestep);

		void Clear();

		void SetParameters(vec2 gravity, int collisionResolutionSteps, float allowedPenetration, float penetrationBiasFactor, float minRigidbodySpeed);
};

#endif
