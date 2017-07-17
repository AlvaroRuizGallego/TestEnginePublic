#ifndef COLLISION_QUERIER_H
#define COLLISION_QUERIER_H

#include "CollisionData.h"
#include "Mask.h"

#include "glm.hpp"

#include <vector>

using namespace glm;

class __declspec(dllexport) CollisionQuerier
{
	private:
		struct _LineImplicitEquation
		{
			//Ax + By = D;
			//(A, B) -> normal to line (doesnt need to be normalized)

			float A;
			float B;
			float D;

			_LineImplicitEquation(vec2 point, vec2 direction)
			{
				vec2 normal = vec2(-direction.y, direction.x);

				A = normal.x;
				B = normal.y;
				D = A*point.x + B*point.y;
			}
		};

		class _Edge
		{
			public:
				vec2 vertexA, vertexB;
				int index;

				_Edge() {}

				_Edge(vec2 vertexA, vec2 vertexB, int index)
				{
					_Edge::vertexA = vertexA;
					_Edge::vertexB = vertexB;
					_Edge::index = index;
				}

				vec2 GetAsVector()
				{
					return vertexB - vertexA;
				}

				vec2 GetNormal()
				{
					vec2 vector = GetAsVector();
					return normalize(vec2(-vector.y, vector.x));
				}
		};


		//Instance
		static CollisionQuerier* _instance;


		CollisionQuerier();

		//Raycast
		bool checkIntersection(_LineImplicitEquation lineA, _LineImplicitEquation lineB, vec2 & intersectionPoint);
		bool pointInSegment(vec2 point, vec2 segmentPointA, vec2 segmentPointB);

		//Collision
		bool checkCollisionBroadPhase(Collider* colliderA, Collider* colliderB);

		bool checkCollisionGJK(Collider* colliderA, Collider* colliderB, std::vector<Contact> & contacts);
		glm::vec2 getMinkowskiSupportPoint(Collider* colliderA, Collider* colliderB, glm::vec2 direction);
		std::vector<Contact> getContactsEPA(Collider* colliderA, Collider* colliderB, std::vector<vec2> simplex);

		bool checkCollisionSAT(Collider* colliderA, Collider* colliderB, std::vector<Contact> & contacts);

		//Clipping
		std::vector<Contact> getContactPoints(Collider* colliderA, Collider* colliderB, vec2 normal);
		_Edge getFeatureEdge(Collider* collider, vec2 direction);
		std::vector<Contact> clip(_Edge clippedEdge, _Edge clippingEdge, Collider* clippingCollider, vec2 normal);
		
	public:
		~CollisionQuerier();

		std::vector<RaycastHit> RaycastAll(glm::vec2 origin, glm::vec2 direction, bool ignoreBackfaces = true, float distance = std::numeric_limits<float>().max(), Mask mask = Mask::Default());
		std::vector<RaycastHit> RaycastCollider(glm::vec2 origin, glm::vec2 direction, Collider* collider, bool ignoreBackfaces = true, float distance = std::numeric_limits<float>().max(), Mask mask = Mask::Default());
		bool CheckCollision(Collider* colliderA, Collider* colliderB, std::vector<Contact> & contacts);

		static CollisionQuerier* GetInstance();
		
};

#endif
