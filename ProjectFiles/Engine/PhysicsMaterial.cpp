#include "PhysicsMaterial.h"

PhysicMaterial::PhysicMaterial()
{
	this->restitution = 0.0f;
	this->friction = 0.0f;
}

PhysicMaterial::PhysicMaterial(float restitution, float friction)
{
	this->restitution = restitution;
	this->friction = friction;
}
