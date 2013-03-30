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
    bool isLight;

	Particle();
	void update();
    void render(vector<vec3>& vtxArray, vector<float>& texArray, vector<color>& colArray);
    void renderLight(vector<vec3>& vtxArray, vector<float>& texArray, vector<color>& colArray);
    float getZOrder() const;
};

void setCameraVec(vec3 cv, bool r);
inline bool operator<(const Particle& a, const Particle& b)
{
	return a.getZOrder() > b.getZOrder();
}

extern vec3 cameraVec;

#endif
