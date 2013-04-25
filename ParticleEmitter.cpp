#include "ParticleEmitter.h"

#include "Actor.h"
#include "Scene.h"

RandomVec::RandomVec()
{
    type = NONE;
    this->lol = 0;
}

RandomVec::RandomVec(float rad)
{
    type = SPHERE;
    this->rad = rad;
    this->lol = 0;
}

RandomVec::RandomVec(float rad, int type)
{
    this->type = type;
    this->rad = rad;
    this->lol = 0;
}

RandomVec::RandomVec(float sx, float sy, float sz)
{
    this->type = BOX;
    this->sx = sx;
    this->sy = sy;
    this->sz = sz;
    this->lol = 0;
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

        float angle2= frand(2*M_PI);
        float z = frand(1);
        float mult = sqrt(1-z*z);
        vec3 randLol(cos(angle2)*mult*lol, sin(angle2)*mult*lol, z*lol);

        switch(type)
        {
        case CIRCLE_XY: return vec3(x, y, 0)+randLol;
        case CIRCLE_YZ: return vec3(0, x, y)+randLol;
        case CIRCLE_XZ: return vec3(x, 0, y)+randLol;
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
    a = vec3(0, 0, 0);
    v = vec3(0, 0, 0);

    lightPermil = 30;

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

vec3 colorClamp(vec3 v)
{
    if(v.x < 0) v.x = 0; if(v.x > 1) v.x = 1;
    if(v.y < 0) v.y = 0; if(v.y > 1) v.y = 1;
    if(v.z < 0) v.z = 0; if(v.z > 1) v.z = 1;
    return v;
}

void ParticleEmitter::spawnParticle(float t)
{
    Particle pt;
    pt.p = act->oldp * (1-t) + act->p * t + randPos.get()*act->particlePosMult*act->sc->particlePosMult;
    pt.v = v + act->v*actorVelMult + randVel.get();
    pt.a = a;
    pt.isLight = (rand()) % 1000 < lightPermil;
	pt.life = life + frand(randLife);
    if(pt.life < 0) return;
	pt.startingLife = pt.life;
	pt.startSize = startSize;
    pt.endSize = endSize;
    pt.startCol = colorClamp(startCol+randCol.get());
    pt.endCol = colorClamp(endCol+randCol.get());
	pt.startAlpha = startAlpha;
	pt.endAlpha = endAlpha;
	
	act->sc->addParticle(pt);
}

void ParticleEmitter::renderLight(vector<vec3>& vtxArray, vector<float>& texArray, vector<color>& colArray)
{
    float size = (startSize + endSize) * (startAlpha + endAlpha) * period * (life+randLife)* 10000.0;
    size = 0.3;
    vec3 col = (startCol+endCol)*0.5f;
    color colr;
    colr.r = col.x;
    colr.g = col.y;
    colr.b = col.z;
    colr.a = 1;

    vec3 cameraVec = act->sc->cameraLookAt - act->sc->cameraPos;
    normalize(cameraVec);
    vec3 dx = cross(cameraVec, vec3(0, 1, 0));
    vec3 dy = cross(dx, cameraVec);
    normalize(dx);
    normalize(dy);
    float sz = size * 60.0f;
    dx *= sz;
    dy *= sz;
    vtxArray.push_back(act->p+dx+dy);
    vtxArray.push_back(act->p+dx-dy);
    vtxArray.push_back(act->p-dx-dy);
    vtxArray.push_back(act->p-dx+dy);

    for(int i = 0; i < 4; i++)
    {
        texArray.push_back(act->p.x);
        texArray.push_back(act->p.y);
        texArray.push_back(act->p.z);
        texArray.push_back(size);
    }

    colArray.push_back(colr);
    colArray.push_back(colr);
    colArray.push_back(colr);
    colArray.push_back(colr);
}
