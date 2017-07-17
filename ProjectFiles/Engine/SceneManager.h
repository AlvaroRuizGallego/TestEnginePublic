#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <map>


class Scene;

#define REGISTER_SCENE(T) bool sceneAux##T = SceneManager::GetInstance()->RegisterScene<T>(#T) && printf("\nSceneManager -> " #T " registered");

class __declspec(dllexport) SceneManager
{
	private:

		static SceneManager* _instance;
		std::map<std::string, Scene*> _sceneMap;

		SceneManager();


	public:

		~SceneManager();

		static SceneManager* GetInstance();
		template<typename T> Scene* RegisterScene(std::string  name)
		{
			_sceneMap.emplace(std::string(name), new T());
			return GetScene(name);
		}

		Scene* GetScene(std::string  name);
};

#endif