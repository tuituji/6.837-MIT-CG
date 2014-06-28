#include "Material.h"
Material::Material( const Vector3f& d_color ,const Vector3f& s_color, float s,
	float r):
diffuseColor( d_color),refractionIndex(r),shininess(s),specularColor(s_color)
{
}

Material::~Material()
{
}

Vector3f Material::getDiffuseColor() const 
{ return  diffuseColor;}

Vector3f Material::Shade( const Ray& ray, const Hit& hit,
	const Vector3f& dirToLight, const Vector3f& lightColor ) {
    Vector3f kd;
		if(t.valid() && hit.hasTex){
			Vector2f texCoord = hit.texCoord;
			Vector3f texColor = t(texCoord[0],texCoord[1]);
			kd = texColor;
		}else{
      kd = this->diffuseColor;
    }
		Vector3f n = hit.getNormal().normalized();
		//Diffuse Shading
		if(noise.valid()){
			kd = noise.getColor(ray.getOrigin()+ray.getDirection()*hit.getT());
		}
		Vector3f color = clampedDot( dirToLight ,n )*pointwiseDot( lightColor , kd);
		return color;
}

Vector3f Material::pointwiseDot( const Vector3f& v1 , const Vector3f& v2 ) {
	Vector3f out = Vector3f( v1[0]*v2[0] , v1[1]*v2[1] , v1[2]*v2[2]);
	return out;
}

float Material::clampedDot( const Vector3f& L , const Vector3f& N )const {
	float d = Vector3f::dot(L,N);
	return (d>0)? d : 0 ; 
}
void Material::loadTexture(const char * filename){
	t.load(filename);
}
float Material::getRefractionIndex(){
	return refractionIndex;
}
Vector3f Material::getDiffuseColor(){
	return diffuseColor;
}
Vector3f Material::getSpecularColor(){
	return specularColor;
}

void Material::setNoise(const Noise & n)
{
	noise=n;
}
