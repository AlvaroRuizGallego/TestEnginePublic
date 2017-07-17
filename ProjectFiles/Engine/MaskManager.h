#ifndef MASK_MANAGER_H
#define MASK_MANAGER_H

#include "Mask.h"

#include <limits>
#include <map>

class __declspec(dllexport) MaskManager
{
	private:
		static MaskManager* _instance;

		unsigned int _currentMaskIndex;
		std::map<std::string, Mask> _maskMap;

		MaskManager();


	public:
		static const unsigned int MAX_MASKS = std::numeric_limits<decltype(Mask::_bits)>::digits;

		~MaskManager();

		static MaskManager* GetInstance();

		void CreateMask(std::string name);
		Mask GetMask(std::string name);
}; 

#endif
