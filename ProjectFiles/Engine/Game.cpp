#include "Game.h"
#include "_RenderingEngine.h"
#include "_AudioEngine.h"
#include "_PhysicsEngine.h"
#include "_Window.h"
#include "_InputManager.h"
#include "Scene.h"
#include "Camera.h"
#include "_FontLoader.h"
#include "Random.h"
#include "Timer.h"
#include "SpriteSheetManager.h"
#include "Drawer.h"
#include "SceneManager.h"

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>


Game* Game::_instance = nullptr;

Game::Game()
{
	//Default values for configuration variables
	_windowWidth = 640;
	_windowHeight = 480;
	_referenceWindowWidth = 640;
	_referenceWindowHeight = 480;
	_maxCameraDepth = 1;
	_windowMode = WINDOWED;
	_windowTitle = "";
	_screenFitMode = Camera::_ScreenFitMode::BEST_FIT;
	_fixedTimestep = 0.01f;
	_gravity = vec2(0, -1000);
	_collisionResolutionSteps = 10;
	_allowedPenetration = 0.1;
	_penetrationBiasFactor = 0.1;
	_minRigidbodySpeed = 5;
	_textureFilteringMode = GL_NEAREST;
	_fontSize = 100;
	_minAttenuationDistance = 640;
	_maxAttenuationDistance = 1280;
	_minPanningDistance = 640;
	_maxPanningDistance = 1280;
	_masterGain = 1;
	_samples = 0;

	_currentScene = nullptr;
	_terminate = false;
	_initialized = false;
	_changeScene = false;
}


Game::~Game()
{
	_instance = nullptr;

	glfwTerminate();
}

void Game::parseConfigurationFile(std::string  fileName)
{
	std::string line;
	std::ifstream configurationFile(fileName);

	if (!configurationFile.good())
	{
		printf("\nWARNING: Game::parseConfigurationFile -> Can't load configuration file");
		return;
	}

	printf("\n\nConfiguration file loaded correctly\n-----------------------------------\n");

	getline(configurationFile, line);
	while (!configurationFile.eof())
	{
		if (line[0] != '#' && line.size() > 0)
		{
			line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

			int pos = (int)line.find("=", 0);
			std::string variable = line.substr(0, pos);
			std::string value = line.substr(pos + 1, line.size() - pos - 1);

			if (variable == "windowWidth")
			{
				_windowWidth = atoi(value.c_str());

				printf("Window width: %i\n", _windowWidth);
			}
			else if (variable == "windowHeight")
			{
				_windowHeight = atoi(value.c_str());

				printf("Window heigth: %i\n", _windowHeight);
			}
			else if (variable == "referenceWindowWidth")
			{
				_referenceWindowWidth = atoi(value.c_str());

				printf("Reference window width: %i\n", _referenceWindowWidth);
			}
			else if (variable == "referenceWindowHeight")
			{
				_referenceWindowHeight = atoi(value.c_str());

				printf("Reference window heigth: %i\n", _referenceWindowHeight);
			}
			else if (variable == "screenFitMode")
			{
				if (value == "bestFit")
				{
					printf("Screen fit mode: Best Fit\n");
					_screenFitMode = Camera::_ScreenFitMode::BEST_FIT;
				}
				else if (value == "widthFit")
				{
					printf("Screen fit mode: Width Fit\n");
					_screenFitMode = Camera::_ScreenFitMode::WIDTH_FIT;
				}
				else if (value == "heightFit")
				{
					printf("Screen fit mode: Height Fit\n");
					_screenFitMode = Camera::_ScreenFitMode::HEIGHT_FIT;
				}
				else if (value == "expand")
				{
					printf("Screen fit mode: Expand\n");
					_screenFitMode = Camera::_ScreenFitMode::EXPAND;
				}
			}
			else if (variable == "maxCameraDepth")
			{
				_maxCameraDepth = atof(value.c_str());

				printf("Max camera depth: %g\n", _maxCameraDepth);
			}
			else if (variable == "windowMode")
			{
				if (value == "windowed")
				{
					printf("Window mode: Windowed\n");
					_windowMode = WINDOWED;
				}
				else if (value == "fullscreen")
				{
					printf("Window mode: Fullscreen\n");
					_windowMode = FULLSCREEN;
				}
			}
			else if (variable == "windowTitle")
			{
				_windowTitle = value;

				printf("Window title: %s\n", _windowTitle.c_str());
			}
			else if (variable == "fixedTimestep")
			{
				_fixedTimestep = (float)atof(value.c_str());

				printf("Fixed Timestep: %g\n", _fixedTimestep);
			}
			else if (variable == "gravityX")
			{
				_gravity.x = atof(value.c_str());

				printf("Gravity X: %g\n", _gravity.x);
			}
			else if (variable == "gravityY")
			{
				_gravity.y = atof(value.c_str());

				printf("Gravity Y: %g\n", _gravity.y);
			}
			else if (variable == "collisionResolutionSteps")
			{
				_collisionResolutionSteps = atoi(value.c_str());

				printf("Collision resolution steps: %i\n", _collisionResolutionSteps);
			}
			else if (variable == "allowedPenetration")
			{
				_allowedPenetration = (float)atof(value.c_str());

				printf("Allowed penetration: %g\n", _allowedPenetration);
			}
			else if (variable == "penetrationBiasFactor")
			{
				_penetrationBiasFactor = (float)atof(value.c_str());

				printf("Penetration bias factor: %g\n", _penetrationBiasFactor);
			}
			else if (variable == "minRigidbodySpeed")
			{
				_minRigidbodySpeed = (float)atof(value.c_str());

				printf("Min rigidbody speed: %g\n", _minRigidbodySpeed);
			}
			else if (variable == "firstScene")
			{
				_firstSceneName = value;

				printf("First scene name: %s\n", _firstSceneName.c_str());
			}
			else if (variable == "textureFilteringMode")
			{
				if (value == "nearest")
				{
					printf("Texture filtering mode: Nearest\n");
					_textureFilteringMode = GL_NEAREST;
				}
				else if (value == "linear")
				{
					printf("Texture filtering mode: Linear\n");
					_textureFilteringMode = GL_LINEAR;
				}
			}
			else if (variable == "fontSize")
			{
				_fontSize = (int)atoi(value.c_str());

				printf("Font size: %i\n", _fontSize);
			}
			else if (variable == "minAttenuationDistance")
			{
				_minAttenuationDistance = atof(value.c_str());

				printf("Min attenuation distance: %g\n", _minAttenuationDistance);
			}
			else if (variable == "maxAttenuationDistance")
			{
				_maxAttenuationDistance = atof(value.c_str());

				printf("Max attenuation distance: %g\n", _maxAttenuationDistance);
			}
			else if (variable == "minPanningDistance")
			{
				_minPanningDistance = atof(value.c_str());

				printf("Min panning distance: %g\n", _minPanningDistance);
			}
			else if (variable == "maxPanningDistance")
			{
				_maxPanningDistance = atof(value.c_str());

				printf("Max panning distance: %g\n", _maxPanningDistance);
			}
			else if (variable == "masterGain")
			{
				_masterGain = atof(value.c_str());

				printf("Master gain: %g\n", _masterGain);
			}
			else if (variable == "multisampling")
			{
				_samples  = atoi(value.c_str());

				printf("Multisampling: %i\n", _samples);
			}
			else
			{
				printf("Variable %s not valid\n", variable.c_str());
			}
		}
		getline(configurationFile, line);
	}

	printf("\n-----------------------------------");


	configurationFile.close();
}



void Game::onButtonTrigger(int button, int action)
{
	if (action == GLFW_PRESS)
	{
		onButtonDown(button);
	}
	else if (action == GLFW_RELEASE)
	{
		onButtonUp(button);
	}
}

void Game::onButtonDown(int button)
{
	_currentScene->OnButtonDown(button);
}

void Game::onButtonUp(int button)
{
	_currentScene->OnButtonUp(button);
}

void Game::onControllerButtonTrigger(int controller, int button, int action)
{
	if (action == GLFW_PRESS)
	{
		onControllerButtonDown(controller, button);
	}
	else if (action == GLFW_RELEASE)
	{
		onControllerButtonUp(controller, button);
	}
}

void Game::onControllerButtonDown(int controller, int button)
{
	_currentScene->OnControllerButtonDown(controller, button);
}

void Game::onControllerButtonUp(int controller, int button)
{
	_currentScene->OnControllerButtonUp(controller, button);
}

void Game::onMouseButtonTrigger(int button, int action, double x, double y)
{
	if (action == GLFW_PRESS)
	{
		onMouseButtonDown(button, x, _window->GetHeight() - y);
	}
	else if (action == GLFW_RELEASE)
	{
		onMouseButtonUp(button, x, _window->GetHeight() - y);
	}
}

void Game::onMouseButtonDown(int button, double x, double y)
{
	glm::vec2 worldPosition = Camera::GetActiveCamera()->ScreenToWorldPosition({ x, y });
	_currentScene->OnMouseButtonDown(button, { x, y }, worldPosition);
}

void Game::onMouseButtonUp(int button, double x, double y)
{
	glm::vec2 worldPosition = Camera::GetActiveCamera()->ScreenToWorldPosition({ x, y });
	_currentScene->OnMouseButtonUp(button, { x, y }, worldPosition);
}

void Game::onMouseMoveTrigger(double x, double y)
{
	onMouseMove(x, _window->GetHeight() - y);
}

void Game::onMouseMove(double x, double y)
{
	glm::vec2 worldPosition = Camera::GetActiveCamera()->ScreenToWorldPosition({ x, y });
	_currentScene->OnMouseMove({ x, y }, worldPosition);
}

void Game::setScene(std::string  scene)
{
	printf("\n\nLOAD SCENE: %s", scene.c_str());

	Scene* nextScene = SceneManager::GetInstance()->GetScene(scene);

	if (nextScene == nullptr)
	{
		printf("\nWARNING: Game::setScene -> Scene not valid");
		return;
	}

	if (_currentScene != nullptr)
	{
		_currentScene->Terminate();
	}

	clear();

	_currentScene = nextScene;
	_currentScene->Initialize();
	_changeScene = false;

	_previousTime = Timer::GetInstance()->GetTime();
}

void Game::ChangeToScene(std::string  scene)
{
	_changeScene = true;
	_nextScene = scene;
}

int Game::getWindowWidth()
{
	return _window->GetWidth();
}

int Game::getWindowHeight()
{
	return _window->GetHeight();
}

int Game::getReferenceWindowWidth()
{
	return _referenceWindowWidth;
}

int Game::getReferenceWindowHeight()
{
	return _referenceWindowHeight;
}

void Game::onTerminate()
{
	delete _audioEngine;
	delete _renderingEngine;
	delete _inputManager;
	delete _physicsEngine;
	delete _window;
}

void Game::clear()
{
	_renderingEngine->Clear();
	_audioEngine->Clear();
	_physicsEngine->Clear();
	_fontLoader->Clear();
	SpriteSheetManager::GetInstance()->Clear();
	Drawer::GetInstance()->Clear();
}

Game * Game::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new Game();
	}

	return _instance;
}

void Game::Initialize(std::string  configurationFile)
{
	if (_initialized) return;
	_initialized = true;

	//Seed rand
	Random::Seed();

	//Configuration file
	parseConfigurationFile(configurationFile);
	
	//GLFW init
	glfwSetErrorCallback([](int error, const char*  description)
	{
		printf("\nWARNING: GLFW Error occurred, Error ID: %i, Description: %s", error, description);
	});

	if (!glfwInit())
	{
		printf("\nERROR: Game::Initialize -> GLFW initialization failed");
		fflush(stdout);
		exit(EXIT_FAILURE);
	}

	//Window
	if (_windowMode == WindowMode::FULLSCREEN)
	{
		const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		_windowWidth = mode->width;
		_windowHeight = mode->height;
	}
	_window = new _Window(_windowWidth, _windowHeight, _windowTitle.c_str(), _windowMode, _samples);

	//Camera
	Camera::_windowWidth = _windowWidth;
	Camera::_windowHeight = _windowHeight;
	Camera::_referenceWindowWidth = _referenceWindowWidth;
	Camera::_referenceWindowHeight = _referenceWindowHeight;
	Camera::_maxDepth = _maxCameraDepth;
	Camera::_screenFitMode = _screenFitMode;

	//Renderer
	_renderingEngine = _RenderingEngine::GetInstance();
	_renderingEngine->Initialize(_window, _textureFilteringMode);

	//Text
	_fontLoader = _FontLoader::GetInstance();
	_fontLoader->SetFontSize(_fontSize);

	//Audio
	_audioEngine = _AudioEngine::GetInstance();
	_audioEngine->SetParameters(_masterGain, _minAttenuationDistance, _maxAttenuationDistance, _minPanningDistance, _maxPanningDistance);

	//Physics
	_physicsEngine = _PhysicsEngine::GetInstance();
	_physicsEngine->SetParameters(_gravity, _collisionResolutionSteps, _allowedPenetration, _penetrationBiasFactor, _minRigidbodySpeed);

	//Input
	_inputManager = _InputManager::GetInstance();
	_inputManager->SetWindow(_window);
	_inputManager->SetButtonCallback(std::bind(&Game::onButtonTrigger, this, std::placeholders::_1, std::placeholders::_2));
	_inputManager->SetControllerButtonCallback(std::bind(&Game::onControllerButtonTrigger, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	_inputManager->SetMouseButtonCallback(std::bind(&Game::onMouseButtonTrigger, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	_inputManager->SetMouseMoveCallback(std::bind(&Game::onMouseMoveTrigger, this, std::placeholders::_1, std::placeholders::_2));

	//Scene
	setScene(_firstSceneName);

	//Time variables
	double timeAccumulator = 0;

	//Main loop
	while (!_terminate && !_window->ShouldClose())
	{
		if (_currentScene == nullptr) continue;
		if (_changeScene) setScene(_nextScene);

		double currentTime = Timer::GetInstance()->GetTime();
		double deltaTime = currentTime - _previousTime;
		_previousTime = currentTime;

		timeAccumulator += deltaTime;

		_inputManager->Update();

		while (timeAccumulator >= _fixedTimestep)
		{
			_currentScene->FixedUpdate(_fixedTimestep);

			_physicsEngine->FixedUpdate(_fixedTimestep);

			timeAccumulator -= _fixedTimestep;
		}

		_currentScene->Update(deltaTime);

		_renderingEngine->Update();

		glfwPollEvents();
	}

	onTerminate();
}

void Game::Terminate()
{
	if (!_initialized) return;

	_terminate = true;
}



