#ifndef PHYSICS_MATERIAL_H
#define PHYSICS_MATERIAL_H


struct __declspec(dllexport) PhysicMaterial 
{
	public:
		float restitution;
		float friction;

		PhysicMaterial();
		PhysicMaterial(float restitution, float friction);
};

#endif
