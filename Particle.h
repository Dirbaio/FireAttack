#ifndef _PARTICLES_H
#define _PARTICLES_H

#include "util.h"

class Particle {
	public:
	
	vec3 p;
	vec3 v;
	vec3 a;

	float life, startingLife;
	
	float startSize, endSize;
	vec3 startCol, endCol;
	float startAlpha, endAlpha;
	
	Particle();
	void render();
	void update();
	void renderArray(vector<vec3>& vtxArray, vector<vec2>& texArray, vector<color>& colArray);
	float getZOrder() const;
};

void setCameraVec(vec3 cv, bool r);
inline bool operator<(const Particle& a, const Particle& b)
{
	return a.getZOrder() > b.getZOrder();
}

extern vec3 cameraVec;

#endif
