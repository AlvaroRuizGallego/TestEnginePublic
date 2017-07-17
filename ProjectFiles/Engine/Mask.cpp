#include "Mask.h"

#include <limits>


Mask::Mask(unsigned long bits)
{
	_bits = bits;
}

Mask Mask::Default()
{
	return Mask(1);
}

Mask Mask::All()
{
	return Mask(std::numeric_limits<decltype(Mask::_bits)>::max());
}

Mask Mask::None()
{
	return Mask(0);
}

Mask::Mask()
{
	*this = Mask::Default();
}

Mask Mask::operator+(const Mask & otherMask)
{
	Mask mask = Mask(_bits | otherMask._bits);

	return mask;
}

Mask Mask::operator+=(const Mask & otherMask)
{
	_bits = _bits | otherMask._bits;
	return *this;
}

bool Mask::Matches(Mask otherMask)
{
	return (_bits & otherMask._bits);
}
