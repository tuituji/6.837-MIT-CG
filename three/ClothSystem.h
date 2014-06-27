#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vecmath.h>
#include <vector>

#include "particleSystem.h"

class ClothSystem: public ParticleSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem();
	vector<Vector3f> evalF(vector<Vector3f> state);
	
	void draw();

private:

};


#endif
