#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Component.h"

#include <glm.hpp>

#include <limits.h>

using namespace glm;

class __declspec(dllexport) Rigidbody: public Component
{
	friend class _PhysicsEngine;
	friend class Collider;

	private:
		//Gravity
		float _gravityScale;

		//Linear
		float _mass; 
		vec2 _linearVelocity;
		vec2 _linearAcceleration;
		vec2 _netForce;
		vec2 _netLinearImpulse;

		//Angular
		float _momentOfInertia;
		float _angularSpeed;
		float _angularAcceleration;
		float _netTorque;
		float _netAngularImpulse;

		bool _hasCollider;

		bool _freezeOrientation;
		bool _freezePositionX;
		bool _freezePositionY;

		void integrateForces(float fixedTimestep);
		void applyVelocities(float fixedTimestep);

		void checkCollider();

	public:
		//Functions
		Rigidbody(float mass, float momentOfInertia, float gravityScale = 1);
		~Rigidbody();

		virtual void FixedUpdate(float fixedTimestep) override;

		virtual void OnAddedToGameObject() override;
		virtual void OnRemovedFromGameObject() override;

		struct SpeedLimits 
		{
			float maxPositiveLinearSpeedX = numeric_limits<float>::max();
			float maxNegativeLinearSpeedX = -numeric_limits<float>::max();
			float maxPositiveLinearSpeedY = numeric_limits<float>::max();
			float maxNegativeLinearSpeedY = -numeric_limits<float>::max();
			float maxPositiveAngularSpeed = numeric_limits<float>::max();
			float maxNegativeAngularSpeed = -numeric_limits<float>::max();
		} speedLimits;


		//Force
		void ApplyForce(vec2  force);
		void ApplyTorque(float torque);
		void ApplyForceAtPoint(vec2 force, vec2 point);

		//Impulse
		void ApplyLinearImpulse(vec2 impulse);
		void ApplyAngularImpulse(float impulse);
		void ApplyImpulseAtPoint(vec2 impulse, vec2 point);

		//Getters & Setters
		float GetMass();
		void SetMass(float mass);
		float GetMomentoOfInertia();
		void SetMomentOfInertia(float momentoOfInertia);
		float GetGravityScale();
		void SetGravityScale(float gravityScale);
		
		float GetAngularSpeed();
		void SetAngularSpeed(float angularSpeed);

		vec2 GetLinearVelocity();
		void SetLinearVelocity(vec2 velocity);
		void SetLinearVelocity(float x, float y);
		void SetLinearVelocityX(float x);
		void SetLinearVelocityY(float y);

		
		void FreezeRotation(bool freeze);
		bool IsRotationFreezed();

		void FreezePositionX(bool freeze);
		bool IsPositionXFreezed();

		void FreezePositionY(bool freeze);
		bool IsPositionYFreezed();
};

#endif
