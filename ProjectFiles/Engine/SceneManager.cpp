#include "SceneManager.h"

SceneManager* SceneManager::_instance = nullptr;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	_instance = nullptr;

	for (auto it = _sceneMap.begin(); it != _sceneMap.end(); it++)
	{
		delete it->second;
	}

	_sceneMap.clear();
}

SceneManager * SceneManager::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new SceneManager();
	}

	return _instance;
}

Scene * SceneManager::GetScene(std::string  name)
{
	auto it = _sceneMap.find(std::string(name));
	if (it != _sceneMap.end())
	{
		return _sceneMap.at(std::string(name));
	}

	printf("\nWARNING: SceneManager::GetScene -> Scene not added");
	return nullptr;
}
