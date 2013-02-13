#ifndef _PARTICLEEMITTER_H
#define _PARTICLEEMITTER_H

#include "Particle.h"

class Actor;

class RandomVec
{
    public:

    float rad;
    float sx, sy, sz;
    int type;

    RandomVec();
    RandomVec(float rad);
    RandomVec(float sx, float sy, float sz);
    RandomVec(float rad, int type);

    virtual vec3 get();
};


#define NONE -1
#define CIRCLE_XY 0
#define CIRCLE_YZ 1
#define CIRCLE_XZ 2
#define SPHERE 3
#define BOX 4

class ParticleEmitter
{
	public:
	Actor* act;
    bool active;

    RandomVec randPos;

	vec3 v;
	float actorVelMult;
    RandomVec randVel;

	vec3 a;

	float life;
	float randLife;
		
	float startSize, endSize;
	vec3 startCol, endCol;
	float startAlpha, endAlpha;

	float period;
    ParticleEmitter(Actor* act);

	void update();

	void setOn();
	void turnOnFor(float time);
    void boom(int count);

	private:
    void spawnParticle(float t);
	
	float state;
	float disableTimer; //-1 if no timer
	bool enabled;
	
};

#endif
