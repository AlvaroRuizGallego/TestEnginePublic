#ifndef MASK_H
#define MASK_H


class __declspec(dllexport) Mask
{
	friend class MaskManager;

	private:
		unsigned long _bits;
		Mask(unsigned long bits);

	public:
		static Mask Default();
		static Mask All();
		static Mask None();

		Mask();

		Mask operator+(const Mask& otherMask);
		Mask operator+=(const Mask& otherMask);
		
		bool Matches(Mask otherMask);
};

#endif
