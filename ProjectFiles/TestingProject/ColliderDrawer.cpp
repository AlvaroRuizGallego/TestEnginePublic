#include "ColliderDrawer.h"


#include "Drawer.h"



bool ColliderDrawer::_enabled = true;
bool ColliderDrawer::drawContactPoints = true;

ColliderDrawer::ColliderDrawer(float lineWidth, vec4 color, bool drawCenter, bool drawNormals, bool drawContainingCircle)
{
	_drawCenter = drawCenter;
	_drawNormals = drawNormals;
	_drawContainingCircle = drawContainingCircle;
	_lineWidth = lineWidth;
	_color = color;

	_elapsedTime = 0;
}

ColliderDrawer::~ColliderDrawer()
{
}

void ColliderDrawer::Update(double deltaTime)
{
	if (!_enabled) return;

	//Draw collider
	for (Collider* collider : _colliders)
	{
		vec2 normalPos;

		std::vector<vec2>* vertices = collider->GetVerticesWorldSpace();
		std::vector<vec2>* normals = collider->GetNormalsWorldSpace();

		for (int i = 0; i < vertices->size(); ++i)
		{
			if (i == vertices->size() - 1)
			{
				Drawer::GetInstance()->DrawLine(vertices->at(i), vertices->at(0), _color, _lineWidth);
				normalPos = vertices->at(i) + (vertices->at(0) - vertices->at(i)) * 0.5f;
			}
			else
			{
				Drawer::GetInstance()->DrawLine(vertices->at(i), vertices->at(i + 1), _color, _lineWidth);
				normalPos = vertices->at(i) + (vertices->at(i + 1) - vertices->at(i)) * 0.5f;
			}

			if (_drawNormals)
			{
				Drawer::GetInstance()->DrawLine(normalPos, normalPos + normals->at(i) * 10.0f);
			}
		}

		if (_drawCenter)
		{
			Drawer::GetInstance()->DrawCross((vec2)_gameObject->GetPosition(), 10, true, _color, _lineWidth);
		}

		if (_drawContainingCircle)
		{
			Circle containingCircleWorldSpace = collider->GetContainingCircleWorldSpace();
			for (int j = 0; j < 360; j += 1)
			{
				Drawer::GetInstance()->DrawCross(containingCircleWorldSpace.center + vec2(cos(radians((float)j))*containingCircleWorldSpace.radius, sin(radians((float)j))*containingCircleWorldSpace.radius), _lineWidth);
			}
		}
	}

	//Draw contact points
	for (vec2 contactPoint : _contactPoints)
	{
		Drawer::GetInstance()->DrawCross(contactPoint, 20, true, glm::vec4(1, 0, 0, 1), _lineWidth);
	}

	_elapsedTime += deltaTime;

	if (_elapsedTime > 0.25)
	{
		_contactPoints.clear();
		_elapsedTime = 0;
	}
}

void ColliderDrawer::OnInitialize()
{
	_colliders = _gameObject->GetComponents<Collider>();
}

void ColliderDrawer::OnCollision(ContactInfo contactInfo)
{
	if (!drawContactPoints) return;

	for (Contact contact : contactInfo.contacts)
	{
		_contactPoints.push_back(contact.position);
	}
}

void ColliderDrawer::OnTrigger(ContactInfo contactInfo)
{
	if (!drawContactPoints) return;

	for (Contact contact : contactInfo.contacts)
	{
		_contactPoints.push_back(contact.position);
	}
}

void ColliderDrawer::ColliderDrawersEnabled(bool enabled)
{
	_enabled = enabled;
}