#include "TestComponent.h"

void TestComponent::Update(double deltaTime)
{
	printf("\nUPDATE");
}

void TestComponent::OnInitialize()
{
	printf("\nON INITIALIZE");
}

void TestComponent::OnTriggerEnter(ContactInfo contactInfo)
{
	printf("\nTRIGGER ENTER");
}

void TestComponent::OnTriggerExit(ContactInfo contactInfo)
{
	printf("\nTRIGGER EXIT");
}



