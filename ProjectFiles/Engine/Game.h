#ifndef GAME_H
#define GAME_H

#include "WindowMode.h"
#include "Camera.h"

#include "glm.hpp"

#include <string>

class _RenderingEngine;
class _Window;
class _AudioEngine;
class _PhysicsEngine;
class _InputManager;
class _FontLoader;

using namespace glm;
using namespace std;

class __declspec(dllexport) Game
{
	friend class Scene;

	private:
		//Instance
		static Game* _instance;

		//Configuration variables
		int _windowWidth;
		int _windowHeight;
		int _referenceWindowWidth;
		int _referenceWindowHeight;
		float _maxCameraDepth;
		WindowMode _windowMode;
		std::string _windowTitle;
		Camera::_ScreenFitMode _screenFitMode;
		float _fixedTimestep;
		int _collisionResolutionSteps;
		vec2 _gravity;
		float _allowedPenetration;
		float _penetrationBiasFactor;
		float _minRigidbodySpeed;
		int _textureFilteringMode;
		int _samples;
		int _fontSize;
		float _minAttenuationDistance;
		float _maxAttenuationDistance;
		float _minPanningDistance;
		float _maxPanningDistance;
		float _masterGain;

		

		//Bools
		bool _terminate;
		bool _initialized;

		//Scene
		Scene* _currentScene;
		std::string _firstSceneName;

		std::string _nextScene;
		bool _changeScene;

		//Renderer
		_RenderingEngine* _renderingEngine;

		//Audio Engine
		_AudioEngine* _audioEngine;

		//Input
		_InputManager* _inputManager;

		//Collision Engine
		_PhysicsEngine* _physicsEngine;

		//Fonts
		_FontLoader* _fontLoader;

		//Window
		_Window* _window;

		//Time
		double _previousTime = 0;

		//Constructor
		Game();

		//Method that obtain the values for the configuration variables from the configuration file
		void parseConfigurationFile(std::string  fileName);

		//Input functions
		void onButtonTrigger(int button, int action);
		virtual void onButtonDown(int button);
		virtual void onButtonUp(int button);

		virtual void onControllerButtonTrigger(int controller, int button, int action);
		virtual void onControllerButtonDown(int controller, int button);
		virtual void onControllerButtonUp(int controller, int button);

		void onMouseButtonTrigger(int button, int action, double x, double y);
		virtual void onMouseButtonDown(int button, double x, double y);
		virtual void onMouseButtonUp(int button, double x, double y);

		void onMouseMoveTrigger(double x, double y);
		virtual void onMouseMove(double x, double y);

		void setScene(std::string  scene);
		int getWindowWidth();
		int getWindowHeight();
		int getReferenceWindowWidth();
		int getReferenceWindowHeight();

		void onTerminate();

		void clear();


	public:
		virtual ~Game();

		static Game* GetInstance();

		void Initialize(std::string  fileName);
		void Terminate();

		void ChangeToScene(std::string  scene);
};


#endif
