#ifndef NOISE_H
#define NOISE_H
#include "vecmath.h"
class Noise
{
public:
	Noise();
	Noise(int _octaves ,
		const Vector3f & color1 = Vector3f::ZERO,
		const Vector3f & color2=Vector3f(1,1,1),
		float freq=1, float amp=1);
	Vector3f getColor(const Vector3f & pos);
	bool valid();

	Noise(const Noise & n);

	Vector3f color[2];
	int octaves;
	float frequency;
	float amplitude;
	bool init;
};

#endif