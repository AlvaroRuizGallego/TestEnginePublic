#include "_Projection.h"

#include "_PhysicsEngine.h"


_Projection::_Projection()
{

}

_Projection::_Projection(float min, float max)
{
	this->min = min;
	this->max = max;
}

bool _Projection::Overlaps(_Projection otherProjection)
{
	return max >= otherProjection.min && min <= otherProjection.max;
}

bool _Projection::Contains(_Projection otherProjection)
{
	return max >= otherProjection.max && min <= otherProjection.min;
}

float _Projection::GetOverlap(_Projection otherProjection)
{
	return glm::min(glm::abs(max - otherProjection.min), glm::abs(otherProjection.max - min));
}

float _Projection::GetCenter()
{
	return (min + max) * 0.5f;
}
