#ifndef LIGHT_H
#define LIGHT_H

#include <Vector3f.h>

#include "Object3D.h"

class Light
{
public:

    Light()
    {

    }

    virtual ~Light()
    {

    }

    virtual void getIllumination( const Vector3f& p, Vector3f& dir, Vector3f& col, float& distanceToLight ) const = 0;

};

class DirectionalLight : public Light
{
public:

    DirectionalLight( const Vector3f& d, const Vector3f& c )
    {
        direction = d.normalized();
        color = c;
    }

    ~DirectionalLight()
    {

    }
  ///@param p unsed in this function
  ///@param distanceToLight not well defined because it's not a point light
    virtual void getIllumination( const Vector3f& p, Vector3f& dir, Vector3f& col, float& distanceToLight ) const
    {
        // the direction to the light is the opposite of the
        // direction of the directional light source
        dir = -direction;
        col = color;
		  distanceToLight = FLT_MAX; 
    }

private:

    DirectionalLight(); // don't use

    Vector3f direction;
    Vector3f color;

};

class PointLight : public Light
{
public:

    PointLight( const Vector3f& p, const Vector3f& c,float fall )
    {
        position = p;
        color = c;
      falloff = fall;
    }

    ~PointLight()
    {

    }

    virtual void getIllumination( const Vector3f& p, Vector3f& dir, Vector3f& col, float& distanceToLight ) const
    {
        // the direction to the light is the opposite of the
        // direction of the directional light source
		  dir = (position-p);
      distanceToLight = dir.abs();
		  dir = dir/dir.abs();
      col = color/(1+falloff*distanceToLight*distanceToLight);
    }

private:

    PointLight(); // don't use
    float falloff;
    Vector3f position;
    Vector3f color;

};

#endif // LIGHT_H
