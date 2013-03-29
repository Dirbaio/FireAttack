#include "ParticleEmitter.h"

#include "Actor.h"
#include "Scene.h"

RandomVec::RandomVec()
{
    type = NONE;
}

RandomVec::RandomVec(float rad)
{
    type = SPHERE;
    this->rad = rad;
}

RandomVec::RandomVec(float rad, int type)
{
    this->type = type;
    this->rad = rad;
}

RandomVec::RandomVec(float sx, float sy, float sz)
{
    this->type = BOX;
    this->sx = sx;
    this->sy = sy;
    this->sz = sz;
}

vec3 RandomVec::get()
{
    switch(type)
    {
    case SPHERE:
    {
        float angle = frand(2*M_PI);
        float z = frand(1);
        float mult = sqrt(1-z*z);
        return vec3(cos(angle)*mult*rad, sin(angle)*mult*rad, z*rad);
    }
    case CIRCLE_XY:
    case CIRCLE_YZ:
    case CIRCLE_XZ:
    {
        float angle = frand(2*M_PI);
        float x = cos(angle)*rad;
        float y = sin(angle)*rad;

        switch(type)
        {
        case CIRCLE_XY: return vec3(x, y, 0);
        case CIRCLE_YZ: return vec3(0, x, y);
        case CIRCLE_XZ: return vec3(x, 0, y);
        }
    }
    case BOX:
       return vec3(frand(sx), frand(sy), frand(sz));
    default:
        return vec3(0, 0, 0);
    }
}


ParticleEmitter::ParticleEmitter(Actor* act)
{
	this->act = act;
    this->active = false;

	//Defaults
    this->period = 1/1000.0f;
    life = 2;
    randLife = 0.6;
	startSize = 0.4; endSize = 0.2;
    startAlpha = 1; endAlpha = 0;
	startCol = vec3(0, 0, 1);
    endCol = vec3(0.1, 1, 0);
    actorVelMult = 0;
    a = vec3(0, 0.2, 0);
    v = vec3(0, 0, 0);


	//Private stuff
    count = 0;
    this->state = 0;
	this->disableTimer = -1;
	this->enabled = true;
}

void ParticleEmitter::update()
{
    if(!active)
    {
        act->oldp = act->p;
        active = true;
    }

	if(disableTimer >= 0)
	{
		disableTimer -= dt;
		if(disableTimer < 0)
			enabled = false;
	}
	
	if(!enabled) return;

	state += dt;
    while(state > period)
	{
        state -= period;
        spawnParticle(state/dt);
	}
}

void ParticleEmitter::boom(int count)
{
    for(int i = 0; i < count; i++)
        spawnParticle(0);
}

void ParticleEmitter::spawnParticle(float t)
{
    Particle pt;
    pt.p = act->oldp * (1-t) + act->p * t + randPos.get()*act->particlePosMult*act->sc->particlePosMult;
    pt.v = v + act->v*actorVelMult + randVel.get();
    pt.a = a;
    pt.isLight = (++count) % 100 == 0;
	pt.life = life + frand(randLife);
	pt.startingLife = pt.life;
	pt.startSize = startSize;
	pt.endSize = endSize;
	pt.startCol = startCol;
	pt.endCol = endCol;
	pt.startAlpha = startAlpha;
	pt.endAlpha = endAlpha;
	
	act->sc->addParticle(pt);
}

