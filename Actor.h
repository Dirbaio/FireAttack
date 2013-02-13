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

	Actor(Scene* sc);

    float particlePosMult;

	virtual void render();
	virtual void update();
    virtual bool collided(Actor* b);
    virtual bool collidedWithGround();

    void updateEmitters();
    virtual void explode();
    ParticleEmitter* addEmitter(const ParticleEmitter& e);
};
#endif
