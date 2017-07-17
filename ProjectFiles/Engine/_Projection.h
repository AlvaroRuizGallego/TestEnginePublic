#ifndef _PROJECTION_H
#define _PROJECTION_H

#include <glm.hpp>

using namespace glm;

class _Projection
{
	public:

		float min, max;

		_Projection();
		_Projection(float min, float max);

		bool Overlaps(_Projection otherProjection);
		bool Contains(_Projection otherProjection);
		float GetOverlap(_Projection otherProjection);
		float GetCenter();
};
#endif
