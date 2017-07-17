#ifndef TEST_COMPONENT_H
#define TEST_COMPONENT_H

#include "Component.h"

class TestComponent: public Component
{
	public:
		void Update(double deltaTime) override;
		void OnAddedToGameObject() override;
		void OnInitialize() override;

		void OnTriggerEnter(ContactInfo contactInfo) override;
		void OnTriggerExit(ContactInfo contactInfo) override;
};

#endif