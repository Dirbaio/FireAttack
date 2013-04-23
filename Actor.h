#ifndef _ACTOR_H
#define _ACTOR_H

#include "ParticleEmitter.h"
#include<list>
#include <Box2D/Box2D.h>

using namespace std;

class Scene;
class Actor
{
	public:

	bool alive;
	list<ParticleEmitter> emitters;
	Scene* sc;
    b2Body* body;
	vec3 p, v;
    vec3 oldp;
    bool inmortal;

    Shader* shader;

	Actor(Scene* sc);
	virtual ~Actor();

    float particlePosMult;

    virtual void render();
    virtual bool renderParticle(Particle& p);
    virtual void update();
    virtual void collided(Actor* b);
    virtual bool collidedWithGround();

    void updateEmitters();
    virtual void explode();
    virtual void explodeWater();
    ParticleEmitter* addEmitter(const ParticleEmitter& e);
};
#endif
