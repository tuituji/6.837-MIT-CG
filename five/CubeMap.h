#ifndef CUBEMAP_H
#define CUBEMAP_H
#include "texture.hpp"
#include "Vector3f.h"
class CubeMap{
public:
///@brief assumes a directory containing
///left right up down front back.bmp
CubeMap(const char * dir);
enum FACE
{
	LEFT,RIGHT,UP,DOWN,FRONT,BACK
};
Vector3f operator()(const Vector3f&);
Texture t[6];

};
#endif