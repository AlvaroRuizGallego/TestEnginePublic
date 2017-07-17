#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"
#include "_Projection.h"
#include "PhysicsMaterial.h"
#include "Geometry.h"
#include "Mask.h"

#include "glm.hpp"

#include <vector>

using namespace glm;

class __declspec(dllexport) Collider: public Component
{
	friend class Rigidbody;
	friend class CollisionQuerier;

	private:

		std::vector<vec2> _vertices; //Clockwise order
		std::vector<vec2> _transformedVertices;
		std::vector<vec2> _normals;
		std::vector<vec2> _transformedNormals;

		bool _isTrigger;
		PhysicMaterial _material;
		Mask _mask;

		Circle _containingCircle;
		mat4 _verticesTransform, _normalsTransform;

		Rigidbody* _rigidbody = nullptr;

		vec2 getSupportPoint(vec2 direction, bool worldSpace = true);
		_Projection getProjection(vec2 projectionAxis, bool worldSpace = true);


	public:
		Collider(std::vector<vec2> vertices, bool clockwise, bool isTrigger = false, PhysicMaterial material = PhysicMaterial(), Mask mask = Mask());//If counterclockwise, order will be altered
		~Collider();

		int tag;//TEMP

		virtual void OnInitialize() override;
		virtual void OnAddedToGameObject() override;
		virtual void OnRemovedFromGameObject() override;

		std::vector<vec2>* GetVertices();
		std::vector<vec2>* GetNormals();
		std::vector<vec2>* GetVerticesWorldSpace();
		std::vector<vec2>* GetNormalsWorldSpace();
		
		Circle GetContainingCircle();
		Circle GetContainingCircleWorldSpace();

		bool ContainsPoint(vec2 point);

		bool IsTrigger();
		PhysicMaterial GetPhysicsMaterial();
		Mask GetMask();
		Rigidbody* GetRigidbody();
};

#endif
