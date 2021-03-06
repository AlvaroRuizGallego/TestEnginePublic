#ifndef TEXT_TEST_SCENE_2_H
#define TEXT_TEST_SCENE_2_H

#include "Scene.h"
#include "GameObject.h"
#include "TextLabel.h"


class TextTestScene2 : public Scene
{
	private:

		//Camera
		Camera* _camera;

		//GameObjects
		GameObject* _textGameObject;
		TextLabel* _textLabel;

		virtual void onInitialize() override;
		virtual void onTerminate() override;

		virtual void update(double deltaTime) override;
		virtual void fixedUpdate(float fixedTimestep) override;


		virtual void onButtonDown(int button) override;
		virtual void onButtonUp(int button) override;
		virtual void onMouseButtonDown(int button, glm::vec2 position, glm::vec2 worldPosition) override;
		virtual void onMouseButtonUp(int button, glm::vec2 position, glm::vec2 worldPosition) override;
		virtual void onMouseMove(glm::vec2 position, glm::vec2 worldPosition) override;

	public:

		TextTestScene2();
		~TextTestScene2();
};

REGISTER_SCENE(TextTestScene2);

#endif