#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Ray.h"
#include "Hit.h"
#include "Material.h"

class Object3D
{
public:
	Object3D()
	{
		material = NULL;
	}
	virtual ~Object3D(){}

	Object3D( Material* material){
	this->material = material ; 
	}
	
	virtual bool intersect( const Ray& r , Hit& h, float tmin) = 0;


	char* type;
protected:

	Material* material;
};

#endif

