#ifndef _ARBITER_H
#define _ARBITER_H

#include "Collider.h"
#include "CollisionData.h"
#include "Rigidbody.h"

#include <vector>


struct _ArbiterKey
{
	public:
		Collider* colliderA;
		Collider* colliderB;

		_ArbiterKey(Collider* colliderA, Collider* colliderB)
		{
			if (colliderA < colliderB)
			{
				_ArbiterKey::colliderA = colliderA;
				_ArbiterKey::colliderB = colliderB;
			}
			else
			{
				_ArbiterKey::colliderA = colliderB;
				_ArbiterKey::colliderB = colliderA;
			}
		}
};

inline bool operator < (const _ArbiterKey& a1, const _ArbiterKey& a2)
{
	if (a1.colliderA < a2.colliderA)
		return true;

	if (a1.colliderA == a2.colliderA && a1.colliderB < a2.colliderB)
		return true;

	return false;
}

class _Arbiter
{
	private:
		Collider* _colliderA;
		Collider* _colliderB;

		std::vector<Contact> _contacts;

		float _friction;

		Rigidbody* _rigidbodyA;
		Rigidbody* _rigidbodyB;

		float _iMassA, _iMassB, _iInertiaA, _iInertiaB;


	public:
		_Arbiter(Collider* colliderA, Collider* colliderB, std::vector<Contact> contacts);
		~_Arbiter();

		void UpdateContacts(std::vector<Contact> contacts);
		void PreStep(float fixedTimestep);
		void Step();
		void PostStep();
};

#endif