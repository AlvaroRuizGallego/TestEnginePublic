#include "Rigidbody.h"

#include "CollisionData.h"
#include "Collider.h"
#include "_PhysicsEngine.h"

#include <string>

void Rigidbody::integrateForces(float fixedTimestep)
{
	//LINEAR
	//Half step velocity
	vec2 halfStepLinearVelocity = _linearVelocity + _linearAcceleration * fixedTimestep * 0.5f;

	//Acceleration
	_linearAcceleration = _netForce / _mass;

	//Velocity
	_linearVelocity = halfStepLinearVelocity + _linearAcceleration * fixedTimestep * 0.5f;
	_linearVelocity += _netLinearImpulse / _mass;

	_linearVelocity.x = clamp(_linearVelocity.x, speedLimits.maxNegativeLinearSpeedX, speedLimits.maxPositiveLinearSpeedX);
	_linearVelocity.y = clamp(_linearVelocity.y, speedLimits.maxNegativeLinearSpeedY, speedLimits.maxPositiveLinearSpeedY);

	//ANGULAR
	//Half step speed
	float halfStepAngularSpeed = _angularSpeed + _angularAcceleration * fixedTimestep * 0.5f;

	//Acceleration
	_angularAcceleration = _netTorque / _momentOfInertia;

	//Velocity
	_angularSpeed = halfStepAngularSpeed + _angularAcceleration * fixedTimestep * 0.5f;
	_angularSpeed += _netAngularImpulse / _momentOfInertia;

	_angularSpeed = clamp(_angularSpeed, speedLimits.maxNegativeAngularSpeed, speedLimits.maxPositiveAngularSpeed);


	//Forces reset
	_netForce = _freezePositionY ? vec2() : _PhysicsEngine::GetInstance()->_gravity * _mass * _gravityScale;
	_netTorque = 0;

	//Impulses reset
	_netLinearImpulse = vec2();
	_netAngularImpulse = 0;
}

void Rigidbody::applyVelocities(float fixedTimestep)
{
	if (glm::length(_linearVelocity) > _PhysicsEngine::GetInstance()->_minRigidbodySpeed)
	{
		glm::vec2 velocity2d = _linearVelocity * fixedTimestep + _linearAcceleration* fixedTimestep * fixedTimestep * 0.5f;
		_gameObject->SetPositionX(_gameObject->GetPosition().x + velocity2d.x);
		_gameObject->SetPositionY(_gameObject->GetPosition().y + velocity2d.y);
	}

	//Orientation
	_gameObject->SetOrientation(_gameObject->GetOrientation() + _angularSpeed * fixedTimestep + _angularAcceleration* fixedTimestep * fixedTimestep * 0.5f);
}

void Rigidbody::checkCollider()
{
	std::vector<Collider*> colliders = _gameObject->GetComponents<Collider>();

	_hasCollider = colliders.size() > 0;

	for (Collider* collider : colliders)
	{
		collider->_rigidbody = this;
	}
}

Rigidbody::Rigidbody(float mass, float momentOfInertia, float gravityScale)
{
	_unique = true;

	_mass = mass;
	_momentOfInertia = momentOfInertia;
	_gravityScale = gravityScale;


	_linearVelocity = _linearAcceleration = _netForce = _netLinearImpulse = vec2(0,0);
	_angularSpeed = _angularAcceleration = _netTorque = _netAngularImpulse = 0;

	_freezePositionX = _freezePositionY = _freezeOrientation = false;
}

Rigidbody::~Rigidbody()
{

}

void Rigidbody::FixedUpdate(float fixedTimestep)
{
	if (!_hasCollider)//If it has collider/s, it will be updated from the Physics Engine
	{
		integrateForces(fixedTimestep);
		applyVelocities(fixedTimestep);
	}
}

void Rigidbody::OnAddedToGameObject()
{
	checkCollider();

	for (Collider* collider : _gameObject->GetComponents<Collider>())
	{
		collider->_rigidbody = this;
	}
}

void Rigidbody::OnRemovedFromGameObject()
{
	for (Collider* collider : _gameObject->GetComponents<Collider>())
	{
		collider->_rigidbody = nullptr;
	}
}

void Rigidbody::ApplyForce(vec2 force)
{
	if (_freezePositionX) force.x = 0;
	if (_freezePositionY) force.y = 0;

	_netForce += force;
}

void Rigidbody::ApplyTorque(float torque)
{
	if (!_freezeOrientation)
	{
		_netTorque += torque;
	}
}

void Rigidbody::ApplyForceAtPoint(vec2 force, vec2 point)
{
	vec3 r = vec3(point.x - _gameObject->GetPosition().x, point.y - _gameObject->GetPosition().y, 0);
	vec3 force3 = vec3(force.x, force.y, 0);

	ApplyTorque(-cross(r, force3).z);
	ApplyForce(force);
}

void Rigidbody::ApplyLinearImpulse(vec2 impulse)
{
	if (_freezePositionX) impulse.x = 0;
	if (_freezePositionY) impulse.y = 0;

	_netLinearImpulse += impulse;
}

void Rigidbody::ApplyAngularImpulse(float impulse)
{
	if (!_freezeOrientation)
	{
		_netAngularImpulse += impulse;
	}
}

void Rigidbody::ApplyImpulseAtPoint(vec2 impulse, vec2 point)
{
	vec3 r = vec3(point.x - _gameObject->GetPosition().x, point.y - _gameObject->GetPosition().y, 0);
	vec3 impulse3 = vec3(impulse.x, impulse.y, 0);

	ApplyAngularImpulse(-cross(r, impulse3).z);
	ApplyLinearImpulse(impulse);
}

float Rigidbody::GetMass()
{
	return _mass;
}

void Rigidbody::SetMass(float mass)
{
	_mass = mass;
}

float Rigidbody::GetMomentoOfInertia()
{
	return _momentOfInertia;
}

void Rigidbody::SetMomentOfInertia(float momentoOfInertia)
{
	_momentOfInertia = momentoOfInertia;
}

float Rigidbody::GetGravityScale()
{
	return _gravityScale;
}

void Rigidbody::SetGravityScale(float gravityScale)
{
	_gravityScale = gravityScale;
}

float Rigidbody::GetAngularSpeed()
{
	return _angularSpeed;
}

void Rigidbody::SetAngularSpeed(float angularSpeed)
{
	if (!_freezeOrientation)
	{
		_angularSpeed = angularSpeed;
	}
}

vec2 Rigidbody::GetLinearVelocity()
{
	return _linearVelocity;
}

void Rigidbody::SetLinearVelocity(vec2 velocity)
{
	SetLinearVelocity(velocity.x, velocity.y);
}

void Rigidbody::SetLinearVelocity(float x, float y)
{
	SetLinearVelocityX(x);
	SetLinearVelocityY(y);
}

void Rigidbody::SetLinearVelocityX(float x)
{
	if (!_freezePositionX)
	{
		_linearVelocity.x = x;
	}
}

void Rigidbody::SetLinearVelocityY(float y)
{
	if (!_freezePositionY)
	{
		_linearVelocity.y = y;
	}
}

void Rigidbody::FreezeRotation(bool freeze)
{
	_freezeOrientation = freeze;
}

bool Rigidbody::IsRotationFreezed()
{
	return _freezeOrientation;
}

void Rigidbody::FreezePositionX(bool freeze)
{
	_freezePositionX = freeze;

	if (freeze)
	{
		_linearVelocity.x = 0;
		_netForce.x = 0;
		_linearAcceleration.x = 0;
	}
}

bool Rigidbody::IsPositionXFreezed()
{
	return _freezePositionX;
}

void Rigidbody::FreezePositionY(bool freeze)
{
	_freezePositionY = freeze;

	if (freeze)
	{
		_linearVelocity.y = 0;
		_netForce.y = 0;
		_linearAcceleration.y = 0;
	}
}

bool Rigidbody::IsPositionYFreezed()
{
	return _freezePositionY;
}


