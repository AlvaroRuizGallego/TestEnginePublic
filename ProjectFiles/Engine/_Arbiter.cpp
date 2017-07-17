#include "_Arbiter.h"

#include "_PhysicsEngine.h"


_Arbiter::_Arbiter(Collider * colliderA, Collider * colliderB, std::vector<Contact> contacts)
{
	_colliderA = colliderA;
	_colliderB = colliderB;
	_contacts = contacts;

	_friction = glm::sqrt(_colliderA->GetPhysicsMaterial().friction * _colliderB->GetPhysicsMaterial().friction);

	_rigidbodyA = _colliderA->GetRigidbody();
	_rigidbodyB = _colliderB->GetRigidbody();

	_iMassA = _rigidbodyA == nullptr ? 0 : 1 / _rigidbodyA->GetMass();
	_iInertiaA = _rigidbodyA == nullptr ? 0 : 1 / _rigidbodyA->GetMomentoOfInertia();
	
	_iMassB = _rigidbodyB == nullptr ? 0 : 1 / _rigidbodyB->GetMass();
	_iInertiaB = _rigidbodyB == nullptr ? 0 : 1 / _rigidbodyB->GetMomentoOfInertia();
}

_Arbiter::~_Arbiter()
{
}

void _Arbiter::UpdateContacts(std::vector<Contact> updatedContacts)
{
	_contacts = updatedContacts;
}

void _Arbiter::PreStep(float fixedTimestep)
{
	if (_colliderA->GetRigidbody() == nullptr && _colliderB->GetRigidbody() == nullptr) return;
	if (_colliderA->IsTrigger() || _colliderB->IsTrigger()) return;

	float allowedPenetration = _PhysicsEngine::GetInstance()->_allowedPenetration;
	float biasFactor = _PhysicsEngine::GetInstance()->_penetrationBiasFactor;

	for (int i = 0; i < _contacts.size(); ++i)
	{
		Contact* contact = &_contacts[i];

		vec3 normal = vec3(contact->normal, 0);
		vec3 tangent = vec3(normal.y, -normal.x, 0);

		vec3 rA = vec3(contact->position, 0) - _colliderA->GetGameObject()->GetPosition();
		vec3 rB = vec3(contact->position, 0) - _colliderB->GetGameObject()->GetPosition();

		contact->massNormal = 1 / (((_iMassA + _iMassB) + dot(cross(cross(rA, normal), rA) * _iInertiaA + cross(cross(rB, normal), rB) * _iInertiaB, normal)) * _contacts.size());
		contact->massTangent = 1 / (((_iMassA + _iMassB) + dot(cross(cross(rA, tangent), rA) * _iInertiaA + cross(cross(rB, tangent), rB) * _iInertiaB, tangent)) * _contacts.size());

		contact->bias = biasFactor * max(0.0f, contact->depth - allowedPenetration) / fixedTimestep;


		//Restitution
		float e = min(_colliderA->GetPhysicsMaterial().restitution, _colliderB->GetPhysicsMaterial().restitution);

		if (e == 0)
		{
			contact->restitutionImpulse = vec2();
		}
		else
		{
			vec3 wA = _rigidbodyA == nullptr ? vec3() : vec3(0, 0, -_rigidbodyA->GetAngularSpeed());
			vec3 wB = _rigidbodyB == nullptr ? vec3() : vec3(0, 0, -_rigidbodyB->GetAngularSpeed());
			vec3 vAP = _rigidbodyA == nullptr ? vec3() : vec3(_rigidbodyA->GetLinearVelocity(), 0) + cross(wA, rA);
			vec3 vBP = _rigidbodyB == nullptr ? vec3() : vec3(_rigidbodyB->GetLinearVelocity(), 0) + cross(wB, rB);
			vec3 vR = vBP - vAP;

			if (dot(vR, normal) < 0)
			{
				continue;
			}

			contact->restitutionImpulse = e * (dot(vR, normal) * contact->massNormal) * (vec2)normal;
		}
	}
}

void _Arbiter::Step()
{
	if (_colliderA->GetRigidbody() == nullptr && _colliderB->GetRigidbody() == nullptr) return;
	if (_colliderA->IsTrigger() || _colliderB->IsTrigger()) return;

	for (int i = 0; i < _contacts.size(); ++i)
	{
		Contact* contact = &_contacts[i];

		vec3 normal = vec3(contact->normal, 0);
		vec3 tangent = vec3(normal.y, -normal.x, 0);

		vec3 rA = vec3(contact->position, 0) - _colliderA->GetGameObject()->GetPosition();
		vec3 rB = vec3(contact->position, 0) - _colliderB->GetGameObject()->GetPosition();

		vec3 wA = _rigidbodyA == nullptr ? vec3() : vec3(0, 0, -_rigidbodyA->GetAngularSpeed());
		vec3 vAP = _rigidbodyA == nullptr ? vec3() : vec3(_rigidbodyA->GetLinearVelocity(), 0) + cross(wA, rA);

		vec3 wB = _rigidbodyB == nullptr ? vec3() : vec3(0, 0, -_rigidbodyB->GetAngularSpeed());
		vec3 vBP = _rigidbodyB == nullptr ? vec3() : vec3(_rigidbodyB->GetLinearVelocity(), 0) + cross(wB, rB);

		vec3 vR = vBP - vAP;


		//NORMAL
		float vn = dot(vR, normal);
		float dJn = (vn + contact->bias) * contact->massNormal;

		//Clamp 
		float jn0 = contact->jn;
		contact->jn = max(jn0 + dJn, 0.0f);
		dJn = contact->jn - jn0;

		vec2 jn = dJn * vec2(normal);

		//Apply
		if (_rigidbodyA != nullptr)
		{
			_rigidbodyA->SetLinearVelocity(_rigidbodyA->GetLinearVelocity() + jn * _iMassA);
			_rigidbodyA->SetAngularSpeed(_rigidbodyA->GetAngularSpeed() - cross(rA, vec3(jn, 0)).z * _iInertiaA);
		}
		if (_rigidbodyB != nullptr)
		{
			_rigidbodyB->SetLinearVelocity(_rigidbodyB->GetLinearVelocity() - jn * _iMassB);
			_rigidbodyB->SetAngularSpeed(_rigidbodyB->GetAngularSpeed() + cross(rB, vec3(jn, 0)).z * _iInertiaB);
		}


		//TANGENT
		float vt = dot(vR, tangent);
		float dJt = vt * contact->massTangent;

		//Clamp
		float maxJt = _friction * contact->jn;
		float jt0 = contact->jt;
		contact->jt = clamp(jt0 + dJt, -maxJt, maxJt);
		dJt = contact->jt - jt0;

		vec2 jt = dJt * vec2(tangent);

		//Apply
		if (_rigidbodyA != nullptr)
		{
			_rigidbodyA->SetLinearVelocity(_rigidbodyA->GetLinearVelocity() + jt * _iMassA);
			_rigidbodyA->SetAngularSpeed(_rigidbodyA->GetAngularSpeed() - cross(rA, vec3(jt, 0)).z * _iInertiaA);
		}
		if (_rigidbodyB != nullptr)
		{
			_rigidbodyB->SetLinearVelocity(_rigidbodyB->GetLinearVelocity() - jt * _iMassB);
			_rigidbodyB->SetAngularSpeed(_rigidbodyB->GetAngularSpeed() + cross(rB, vec3(jt, 0)).z * _iInertiaB);
		}
	}
}

void _Arbiter::PostStep()//Restitution
{
	if (_colliderA->GetRigidbody() == nullptr && _colliderB->GetRigidbody() == nullptr) return;
	if (_colliderA->IsTrigger() || _colliderB->IsTrigger()) return;

	for (int i = 0; i < _contacts.size(); ++i)
	{
		Contact* contact = &_contacts[i];

		vec3 rA = vec3(contact->position, 0) - _colliderA->GetGameObject()->GetPosition();
		vec3 rB = vec3(contact->position, 0) - _colliderB->GetGameObject()->GetPosition();

		//Apply restitution
		if (_rigidbodyA != nullptr)
		{
			_rigidbodyA->SetLinearVelocity(_rigidbodyA->GetLinearVelocity() + contact->restitutionImpulse * _iMassA);
			_rigidbodyA->SetAngularSpeed(_rigidbodyA->GetAngularSpeed() - cross(rA, vec3(contact->restitutionImpulse, 0)).z * _iInertiaA);
		}
		if (_rigidbodyB != nullptr)
		{
			_rigidbodyB->SetLinearVelocity(_rigidbodyB->GetLinearVelocity() - contact->restitutionImpulse * _iMassB);
			_rigidbodyB->SetAngularSpeed(_rigidbodyB->GetAngularSpeed() + cross(rB, vec3(contact->restitutionImpulse, 0)).z * _iInertiaB);
		}
	}
}
