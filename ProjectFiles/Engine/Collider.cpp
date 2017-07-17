#include "Collider.h"

#include "_PhysicsEngine.h"
#include "Drawer.h"
#include "Rigidbody.h"


vec2 Collider::getSupportPoint(vec2 direction, bool worldSpace)
{
	vec2 supportPoint;
	float maxDistance = -std::numeric_limits<float>::max();

	std::vector<vec2>* vertices = worldSpace ? GetVerticesWorldSpace() : &_vertices;

	for (int i = 0; i < vertices->size(); i++)
	{
		vec2 vertex = vertices->at(i);
		float distance = dot(direction, vertex);
		if (distance > maxDistance)
		{
			maxDistance = distance;
			supportPoint = vertex;
		}
	}

	return supportPoint;
}

_Projection Collider::getProjection(vec2 projectionAxis, bool worldSpace)
{
	float min = std::numeric_limits<float>().max();
	float max = -min;

	std::vector<vec2>* vertices = worldSpace ? GetVerticesWorldSpace() : &_vertices;

	for (int i = 0; i < vertices->size(); i++)
	{
		vec2 vertex = vertices->at(i);
		float distance = dot(vertex, projectionAxis);

		max = distance > max ? distance : max;
		min = distance < min ? distance : min;
	}

	return _Projection(min, max);
}

Collider::Collider(std::vector<vec2> vertices, bool clockwise, bool isTrigger, PhysicMaterial material, Mask mask)
{
	if (!clockwise)
	{
		std::reverse(vertices.begin(), vertices.end());
	}

	_vertices = vertices;

	float maxDistance = 0;

	vec2 v1Tov2;
	for (int i = 0; i < _vertices.size(); ++i)
	{
		_transformedVertices.push_back(_vertices[i]);

		float distance = glm::length(_vertices[i]);
		if (distance > maxDistance) maxDistance = distance;

		vec2 v1Tov2 = i == _vertices.size() - 1 ? _vertices[0] - _vertices[i] : _vertices[i + 1] - _vertices[i];
		vec2 normal = normalize(vec2(-v1Tov2.y, v1Tov2.x));
		_normals.push_back(normal);
		_transformedNormals.push_back(normal);
	}

	_containingCircle = { { 0,0 },  maxDistance };

	_isTrigger = isTrigger;
	_material = material;
	_mask = mask;

	_rigidbody = nullptr;
}


Collider::~Collider()
{
}

void Collider::OnInitialize()
{
	_PhysicsEngine::GetInstance()->AddCollider(this);
}

void Collider::OnAddedToGameObject()
{
	_rigidbody = _gameObject->GetComponent<Rigidbody>();

	if (_rigidbody != nullptr)
	{
		_rigidbody->_hasCollider = true;
	}
}

void Collider::OnRemovedFromGameObject()
{
	if (_rigidbody != nullptr)
	{
		_rigidbody->checkCollider();//Update _hasCollider flag of rigidbody
		_rigidbody = nullptr;
	}
}

std::vector<vec2>* Collider::GetVertices()
{
	return &_vertices;
}

std::vector<vec2>* Collider::GetNormals()
{
	return &_normals;
}


std::vector<vec2>* Collider::GetVerticesWorldSpace()
{
	if (_verticesTransform == _gameObject->GetTransform()) return &_transformedVertices;//Normals hasn't changed from last time

	_verticesTransform = _gameObject->GetTransform();

	for (int i = 0; i < _transformedVertices.size(); i++)
	{
		_transformedVertices[i] = vec2(_verticesTransform * vec4(_vertices[i].x, _vertices[i].y, 0.0f, 1.0f));
	}

	return &_transformedVertices;
}

std::vector<vec2>* Collider::GetNormalsWorldSpace()
{
	if (_normalsTransform == _gameObject->GetTransform()) return &_transformedNormals;//Normals hasn't changed from last time

	_normalsTransform = _gameObject->GetTransform();

	for (int i = 0; i < _transformedNormals.size(); i++)
	{
		_transformedNormals[i] = normalize(vec2(_normalsTransform * vec4(_normals[i].x, _normals[i].y, 0.0f, 0.0f)));
	}

	return &_transformedNormals;
}

Circle Collider::GetContainingCircle()
{
	return _containingCircle;
}

Circle Collider::GetContainingCircleWorldSpace()
{
	return{ vec2(_gameObject->GetPosition()), _containingCircle.radius * max(_gameObject->GetScale().x, _gameObject->GetScale().y) };
}

bool Collider::ContainsPoint(vec2 point)
{
	std::vector<vec2>* vertices = GetVerticesWorldSpace();
	std::vector<vec2>* normals = GetNormalsWorldSpace();

	for (int i = 0; i < vertices->size(); ++i)
	{
		vec2 normal = normals->at(i);
		vec2 vertex = vertices->at(i);

		vec2 vertex2Point = point - vertex;

		if (dot(normal, vertex2Point) > 0)
		{
			return false;
		}
	}

	return true;
}


bool Collider::IsTrigger()
{
	return _isTrigger;
}

PhysicMaterial Collider::GetPhysicsMaterial()
{
	return _material;
}

Mask Collider::GetMask()
{
	return _mask;
}

Rigidbody * Collider::GetRigidbody()
{
	return _rigidbody;
}

