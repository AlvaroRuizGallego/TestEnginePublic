#include "MaskManager.h"

#include "glm.hpp"

#include <iostream>

MaskManager* MaskManager::_instance = nullptr;


MaskManager::MaskManager()
{
	_currentMaskIndex = 1; //Default is the first (index 0)
}

MaskManager::~MaskManager()
{
	_instance = nullptr;
}

MaskManager * MaskManager::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new MaskManager();
	}

	return _instance;
}

void MaskManager::CreateMask(std::string name)
{
	if (_currentMaskIndex <= MAX_MASKS)
	{
		Mask mask = Mask(glm::pow(2, _currentMaskIndex));
		_currentMaskIndex++;

		_maskMap[name] = mask;
	}
	else
	{
		printf("\nWARNING: MaskManager::CreateMask() -> Cannot create another mask, mask limit reached");
	}
}

Mask MaskManager::GetMask(std::string name)
{
	std::map<std::string, Mask>::iterator it = _maskMap.find(name);
	if (it != _maskMap.end())
	{
		return  it->second;
	}

	printf("\nWARNING: MaskManager::GetMask -> _maskMap doesn't contain the mask named: %s", name.c_str());

	return Mask();
}
