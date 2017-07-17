#ifndef COLLIDER_DRAWER_H
#define COLLIDER_DRAWER_H

#include "Component.h"
#include "Collider.h"
#include "CollisionData.h"

class ColliderDrawer : public Component
{
private:
	vector<Collider*> _colliders;
	//std::vector<ContactInfo> _currentContacts;
	bool _drawCenter, _drawNormals, _drawContainingCircle;
	float _lineWidth;
	vec4 _color;

	bool static _enabled;

	float _elapsedTime;
	std::vector<vec2> _contactPoints;

public:
	ColliderDrawer(float lineWidth = 2, vec4 color = { 1,1,1,1 }, bool drawCenter = false, bool drawNormals = false, bool drawContainingCircle = false);
	~ColliderDrawer();

	static bool drawContactPoints;


	void Update(double deltaTime) override;
	void OnInitialize() override;

	void OnCollision(ContactInfo contactInfo) override;
	void OnTrigger(ContactInfo contactInfo) override;

	void static ColliderDrawersEnabled(bool enabled);
};
#endif