#ifndef _UTIL_H
#define _UTIL_H

#define GL_GLEXT_PROTOTYPES 1

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glext.h"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;
using namespace sf;

typedef Vector3<float> vec3;
typedef Vector2<float> vec2;

struct color {
	float r, g, b, a;
};

//Dot product
inline float dot(const vec3& a, const vec3& b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

//Cross product
inline vec3 cross(const vec3& a, const vec3& b)
{
	return vec3(a.y*b.z - b.y*a.z, 
				-a.x*b.z + b.x*a.z, 
				a.x*b.y - b.x*a.y);
}

//Returns random float between -f and f
inline float frand(float f)
{
	return ((rand()%(1<<20))/float(1<<19) - 1)*f;
}

inline bool prand(float f)
{
    return frand(0.5) < f-0.5;
}

inline float norm(vec3 v)
{
	return sqrt(dot(v, v));
}

inline void normalize(vec3& v)
{
	v /= norm(v);
}

inline float toRad(float deg)
{
    return deg/180.0*M_PI;
}

inline float toDeg(float rad)
{
    return rad*180.0/M_PI;
}

//DELTA TIME!
extern float dt;
extern float tim;
extern Window* theApp;

Texture* loadTexture(string path);
SoundBuffer* loadSound(string path);
Shader* loadShader(string vertex, string fragment);

extern vector<vec3> hexVert, hexNorm;
extern vector<vec2> hexTexCoord;

void renderCube(float x, float y, float z, float sx, float sy, float sz, float rot);

#endif
