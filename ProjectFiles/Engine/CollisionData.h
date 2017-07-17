#ifndef COLLISION_DATA_H
#define COLLISION_DATA_H

#include "glm.hpp"

#include <vector>

class Collider;

struct Contact
{
	glm::vec2 position;
	glm::vec2 normal;
	float depth;

	//Set on collision resolution
	float jn;	
	float jt;	
	float massNormal;
	float massTangent;
	float bias;
	glm::vec2 restitutionImpulse;

	Contact()
	{
		jn = jt = 0;
		restitutionImpulse = glm::vec2();
	}

	Contact(glm::vec2 position, glm::vec2 normal, float depth)
	{
		jn = jt = 0;
		restitutionImpulse = glm::vec2();

		Contact::position = position;
		Contact::normal = normal;
		Contact::depth = depth;
	}
};

struct ContactInfo
{
	std::vector<Contact> contacts;
	Collider* thisCollider;
	Collider* otherCollider;
};


struct RaycastHit
{
	public:
		float distance;
		glm::vec2 contactPoint;
		glm::vec2 normal;
		int edgeIndex;
		Collider* collider;
};

#endif
