#include "CollisionQuerier.h"
#include "_PhysicsEngine.h"
#include "Drawer.h"
#include "Timer.h"

#include <algorithm>

CollisionQuerier* CollisionQuerier::_instance = nullptr;


CollisionQuerier::CollisionQuerier()
{
}

bool CollisionQuerier::checkIntersection(_LineImplicitEquation lineA, _LineImplicitEquation lineB, vec2 & intersectionPoint)
{
	float det = lineA.A*lineB.B - lineB.A*lineA.B;

	if (det == 0) return false;

	intersectionPoint.x = (lineB.B*lineA.D - lineA.B*lineB.D) / det;
	intersectionPoint.y = (lineA.A*lineB.D - lineB.A*lineA.D) / det;

	return true;
}

bool CollisionQuerier::pointInSegment(vec2 point, vec2 segmentPointA, vec2 segmentPointB)
{
	vec2 AB = segmentPointB - segmentPointA;
	vec2 AP = point - segmentPointA;

	float distanceP = dot(AP, AB);
	float squaredDistanceAB = dot(AB, AB);

	return (distanceP >= 0 && distanceP <= squaredDistanceAB);
}

CollisionQuerier::~CollisionQuerier()
{
	_instance = nullptr;
}

std::vector<RaycastHit> CollisionQuerier::RaycastAll(vec2 origin, vec2 direction, bool ignoreBackfaces, float distance, Mask mask)
{
	std::vector<RaycastHit> hits;

	std::vector<Collider*> colliders = _PhysicsEngine::GetInstance()->_colliders;

	for (Collider* collider : colliders)
	{
		for (RaycastHit hit : RaycastCollider(origin, direction, collider, ignoreBackfaces, distance, mask))
		{
			hits.push_back(hit);
		}
	}

	std::sort(hits.begin(), hits.end(), [] (const RaycastHit& lhs, const RaycastHit& rhs) -> bool
	{
		return lhs.distance < rhs.distance;
	});

	return hits;
}


std::vector<RaycastHit> CollisionQuerier::RaycastCollider(glm::vec2 origin, glm::vec2 direction, Collider * collider, bool ignoreBackfaces, float distance, Mask mask)
{
	std::vector<RaycastHit> hits;

	_LineImplicitEquation rayEquation = _LineImplicitEquation(origin, direction);

	if (!collider->GetMask().Matches(mask)) return hits;

	std::vector<vec2>* vertices = collider->GetVerticesWorldSpace();
	std::vector<vec2>* normals = collider->GetNormalsWorldSpace();

	for (int i = 0; i < vertices->size(); i++)
	{
		if (ignoreBackfaces && dot(direction, normals->at(i)) >= 0) continue;

		int j = (i + 1 == vertices->size() ? 0 : i + 1);

		vec2 verticeA = vertices->at(i);
		vec2 verticeB = vertices->at(j);

		_LineImplicitEquation segmentEquation = _LineImplicitEquation(verticeA, verticeB - verticeA);

		vec2 intersectionPoint;

		if (checkIntersection(rayEquation, segmentEquation, intersectionPoint))
		{
			vec2 originToIntersection = intersectionPoint - origin;
			float distanceToIntersection = length(originToIntersection);

			if (distanceToIntersection < distance)
			{
				if (dot(direction, originToIntersection) > 0)
				{
					if (pointInSegment(intersectionPoint, verticeA, verticeB))
					{
						hits.push_back({ distanceToIntersection, intersectionPoint, normals->at(i), i, collider });

						if (ignoreBackfaces || hits.size() == 2)
						{
							break;
						}
					}
				}
			}
		}
	}

	std::sort(hits.begin(), hits.end(), [](const RaycastHit& lhs, const RaycastHit& rhs) -> bool
	{
		return lhs.distance < rhs.distance;
	});

	return hits;
}

bool CollisionQuerier::CheckCollision(Collider * colliderA, Collider * colliderB, std::vector<Contact> & contacts)
{
	return (colliderA->GetMask().Matches(colliderB->GetMask()) && checkCollisionBroadPhase(colliderA, colliderB) && checkCollisionSAT(colliderA, colliderB, contacts));
}

bool CollisionQuerier::checkCollisionBroadPhase(Collider * colliderA, Collider * colliderB)
{
	Circle containingCircleA = colliderA->GetContainingCircleWorldSpace();
	Circle containingCircleB = colliderB->GetContainingCircleWorldSpace();

	float collisionDistance = containingCircleA.radius + containingCircleB.radius;
	vec2 AB = containingCircleB.center - containingCircleA.center;

	return dot(AB, AB) < collisionDistance*collisionDistance;
}


bool CollisionQuerier::checkCollisionGJK(Collider * colliderA, Collider* colliderB, std::vector<Contact> & contacts)
{
	auto directionFromTo = [](vec2 vA, vec2 vB) -> vec2
	{
		vec2 vAn = normalize(vA);
		return vB - vAn*dot(vB, vAn);

		//return (vec2)normalize(cross(cross(vec3(vA.x, vA.y, 0), vec3(vB.x, vB.y, 0)), vec3(vA.x, vA.y, 0)));
	};

	std::vector<vec2> simplex;
	vec2 firstPoint = getMinkowskiSupportPoint(colliderA, colliderB, { 1, 0 });

	simplex.push_back(firstPoint); //Add any point of the minkowski difference (in this case, the furthest away positive x)

	vec2 direction = -firstPoint;
	vec2 nextPoint;

	vec2 A, B, C; //A -> last vertex added to simplex, B-> previous vertex added to simplex, -> ...

	while (true)
	{
		nextPoint = getMinkowskiSupportPoint(colliderA, colliderB, direction);

		if (dot(nextPoint, direction) < 0)//NO INTERSECTION
		{
			return false;
		}

		simplex.push_back(nextPoint);

		if (simplex.size() == 2)
		{
			A = simplex[1];
			B = simplex[0];

			vec2 AB = B - A;
			vec2 AO = -A;

			direction = directionFromTo(AB, AO);
		}
		else//Size == 3
		{
			A = simplex[2];
			B = simplex[1];
			C = simplex[0];

			vec2 AB = B - A;
			vec2 AC = C - A;
			vec2 AO = -A;

			if (dot(-directionFromTo(AC, AB), AO) > 0)//1
			{
				direction = -directionFromTo(AC, AB);

				//Remove B
				simplex.erase(simplex.begin() + 1);
			}
			else//2,3
			{
				if (dot(-directionFromTo(AB, AC), AO) > 0)//2
				{
					direction = -directionFromTo(AB, AC);

					//Remove C
					simplex.erase(simplex.begin());
				}
				else//3
				{
					//INTERSECTION
					contacts = getContactsEPA(colliderA, colliderB, simplex);

					if (contacts.size() == 0)
					{
						return false;
					}

					return true;
				}
			}
		}
	}
}

glm::vec2 CollisionQuerier::getMinkowskiSupportPoint(Collider * colliderA, Collider * colliderB, glm::vec2 direction)
{
	return colliderA->getSupportPoint(direction) - colliderB->getSupportPoint(-direction);
}

std::vector<Contact> CollisionQuerier::getContactsEPA(Collider * colliderA, Collider * colliderB, std::vector<vec2> simplex)
{
	int closestEdgeVertexBIndex = 0;
	float distanceToClosestEdge = 0;

	auto findClosestEdge = [&closestEdgeVertexBIndex, &distanceToClosestEdge](std::vector<vec2> simplex) -> _Edge
	{
		double minDistance = std::numeric_limits<double>().max();
		_Edge minDistanceEdge;

		for (int i = 0; i < simplex.size(); i++)
		{
			int j = (i + 1 == simplex.size() ? 0 : i + 1);

			_Edge edge;
			edge.vertexA = simplex[i];
			edge.vertexB = simplex[j];

			float distance = dot(edge.vertexA, edge.GetNormal());

			if (distance < minDistance)
			{
				minDistance = distance;

				closestEdgeVertexBIndex = j;
				distanceToClosestEdge = minDistance;

				minDistanceEdge = edge;
			}
		}
		return minDistanceEdge;
	};

	auto getClockwise = [](std::vector<vec2> simplex) -> std::vector<vec2>
	{
		//Center
		vec2 center = vec2();
		for (vec2 vertex : simplex) { center += vertex; }
		center /= simplex.size();

		//Sort
		std::sort(simplex.begin(), simplex.end(), [center](vec2 vA, vec2 vB) -> bool
		{
			vec2 cTovA = vA - center;
			vec2 cTovB = vB - center;

			return atan2f(cTovA.x, cTovA.y) < atan2f(cTovB.x, cTovB.y);
		});

		return simplex;
	};

	float tolerance = 0.001f;

	glm::vec2 normal;

	simplex = getClockwise(simplex);

	int iterationsLeft = 20;//In case something goes wrong

	while (iterationsLeft > 0)
	{
		//printf("\nEPA: %i", iterationsLeft);

		_Edge closestEdge = findClosestEdge(simplex);

		vec2 supportPoint = getMinkowskiSupportPoint(colliderA, colliderB, closestEdge.GetNormal());

		if (supportPoint == vec2())//Both colliders are exactly the same and have exactly the same transform
		{
			normal = vec2();
			break;
		}

		double distanceToSupportPoint = dot(supportPoint, closestEdge.GetNormal());

		if (distanceToSupportPoint - distanceToClosestEdge < tolerance)
		{
			normal = -closestEdge.GetNormal();
			break;
		}
		else
		{
			simplex.insert(simplex.begin() + closestEdgeVertexBIndex, supportPoint);
		}

		iterationsLeft--;
	}

	if (iterationsLeft == 0)
	{
		printf("\nEPA FAILED");
		return std::vector<Contact>();
	}

	//Clipping
	return getContactPoints(colliderA, colliderB, normal);
}

bool CollisionQuerier::checkCollisionSAT(Collider * colliderA, Collider * colliderB, std::vector<Contact> & contacts)
{
	std::vector<vec2> axes;

	for (vec2 axis : *colliderA->GetNormalsWorldSpace())
	{
		axes.push_back(axis);
	}

	for (vec2 axis : *colliderB->GetNormalsWorldSpace())
	{
		axes.push_back(axis);
	}

	float minOverlap = std::numeric_limits<float>().max();
	vec2 minOverlapAxis;

	_Projection projectionA, projectionB;
	for (vec2 axis : axes)
	{
		projectionA = colliderA->getProjection(axis);
		projectionB = colliderB->getProjection(axis);

		if (!projectionA.Overlaps(projectionB))//No overlap
		{
			return false;
		}
		else //Overlap
		{
			if (projectionB.GetCenter() > projectionA.GetCenter())
			{
				axis = -axis;
			}

			float overlap = projectionA.GetOverlap(projectionB);
			if (overlap < minOverlap)
			{
				minOverlap = overlap;
				minOverlapAxis = axis;
			}
		}
	}

	//Clipping
	contacts = getContactPoints(colliderA, colliderB, minOverlapAxis);

	if (contacts.size() == 0) return false;

	return true;
}


std::vector<Contact> CollisionQuerier::getContactPoints(Collider* colliderA, Collider* colliderB, vec2 normal)
{
	_Edge e1 = getFeatureEdge(colliderA, -normal);
	_Edge e2 = getFeatureEdge(colliderB, normal);
	_Edge incidentEdge, referenceEdge;

	

	Collider* referenceCollider;

	bool flip = false;

	if (abs(dot(normalize(e1.GetAsVector()), normal)) < abs(dot(normalize(e2.GetAsVector()), normal)))
	{
		referenceEdge = e1;
		incidentEdge = e2;

		referenceCollider = colliderA;
	}
	else
	{
		referenceEdge = e2;
		incidentEdge = e1;

		referenceCollider = colliderB;
	}

	

	std::vector<Contact> contacts =  clip(incidentEdge, referenceEdge, referenceCollider, normal);

	return contacts;
}

CollisionQuerier::_Edge CollisionQuerier::getFeatureEdge(Collider * collider, vec2 direction)
{
	float max = -std::numeric_limits<float>().max();

	std::vector<vec2>* vertices = collider->GetVerticesWorldSpace();
	int farthestVertexIndex;

	for (int i = 0; i < vertices->size(); i++)
	{
		float distance = dot(vertices->at(i), direction);

		if (distance > max)
		{
			max = distance;
			farthestVertexIndex = i;
		}
	}

	vec2 farthestVertex = vertices->at(farthestVertexIndex);

	vec2 leftVertex = farthestVertexIndex == 0 ? vertices->at(vertices->size() - 1) : vertices->at(farthestVertexIndex - 1);
	vec2 rightVertex = farthestVertexIndex == vertices->size() - 1 ? vertices->at(0) : vertices->at(farthestVertexIndex + 1);

	vec2 l = normalize(farthestVertex - leftVertex);
	vec2 r = normalize(farthestVertex - rightVertex);

	if (dot(r, direction) < dot(l, direction))
	{
		return _Edge(farthestVertex, rightVertex, farthestVertexIndex);
	}
	else
	{
		return _Edge(leftVertex, farthestVertex, farthestVertexIndex == 0 ? vertices->size() - 1 : farthestVertexIndex - 1);
	}
}

std::vector<Contact> CollisionQuerier::clip(_Edge clippedEdge, _Edge clippingEdge, Collider* clippingCollider, vec2 normal)
{
	std::vector<Contact> contacts;

	vec2 clippingVector = normalize(clippingEdge.GetAsVector());

	vec2 pointA, pointB;

	pointA = clippedEdge.vertexA;
	pointB = clippedEdge.vertexB;

	std::vector<RaycastHit> hits;

	if (clippingCollider->ContainsPoint(pointA))
	{
		Contact contact;
		contact.position = pointA;
		contact.normal = normal;
		contact.depth = abs(dot(normal, clippingEdge.vertexA) - dot(normal, contact.position));

		contacts.push_back(contact);
	}
	else
	{
		hits = RaycastCollider(pointA, pointB - pointA, clippingCollider, false);

		if (hits.size() > 0)
		{
			Contact contact;
			contact.position = hits[0].contactPoint;
			contact.normal = normal;
			contact.depth = abs(dot(normal, clippingEdge.vertexA) - dot(normal, contact.position));

			contacts.push_back(contact);
		}
	}

	if (clippingCollider->ContainsPoint(pointB))
	{
		Contact contact;
		contact.position = pointB;
		contact.normal = normal;
		contact.depth = abs(dot(normal, clippingEdge.vertexA) - dot(normal, contact.position));

		contacts.push_back(contact);
	}
	else
	{
		hits = RaycastCollider(pointB, pointA - pointB, clippingCollider, false);

		if (hits.size() > 0)
		{
			Contact contact;
			contact.position = hits[0].contactPoint;
			contact.normal = normal;
			contact.depth = abs(dot(normal, clippingEdge.vertexA) - dot(normal, contact.position));

			contacts.push_back(contact);
		}
	}

	return contacts;
}



CollisionQuerier * CollisionQuerier::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new CollisionQuerier();
	}

	return _instance;
}

