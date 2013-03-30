#include "Actor.h"
#include "Scene.h"

Actor::Actor(Scene* sc)
{
	this->sc = sc;
	alive = true;
//	emitters.push_back(ParticleEmitter(this));
    particlePosMult = 1;
    body = NULL;
    shader = NULL;
}

void Actor::update()
{
    if (p.y <= -0.1)
        explodeWater();
}
void Actor::render() {}

void Actor::updateEmitters()
{
    if(body)
    {
        p.x = body->GetPosition().x;
        p.y = body->GetPosition().y;
        p.z = 0;
        v.x = body->GetLinearVelocity().x;
        v.y = body->GetLinearVelocity().y;
        v.z = 0;
    }
	for(list<ParticleEmitter>::iterator it = emitters.begin(); it != emitters.end(); it++)
		it->update();

    oldp = p;
}

ParticleEmitter* Actor::addEmitter(const ParticleEmitter& e)
{
    emitters.push_back(e);
    return &emitters.back();
}

void Actor::explode()
{
    ParticleEmitter e(this);
    e.randVel = RandomVec(3, CIRCLE_XY);
    e.life = 0.3;
    e.startAlpha = 1;
    e.endAlpha = 0;
    e.startCol = vec3(0.3, 0, 0);
    e.endCol = vec3(1, 0.4, 0.2);
    e.startSize = 0;
    e.endSize = 0.3;
    e.boom(500);
    alive = false;
}

void Actor::explodeWater()
{
    ParticleEmitter e(this);
    e.lightPermil = 0;
    e.randVel = RandomVec(3);
    e.life = 1;
    e.startAlpha = 1;
    e.endAlpha = 0;
    e.startCol = vec3(0, 0, 0.4);
    e.endCol = vec3(0, 0, 1);
    e.startSize = 0;
    e.endSize = 1;
    e.boom(500);
    e.randVel = RandomVec(2.5);
    e.life = 1;
    e.startAlpha = 1;
    e.endAlpha = 0;
    e.startCol = vec3(0, 0.3, 0.6);
    e.endCol = vec3(0, 0, 1);
    e.startSize = 0;
    e.endSize = 1;
    e.boom(500);

    e.randVel = RandomVec(0, 5, 0);
    e.randPos = RandomVec(0, 1, 0);
    e.life = 1;
    e.startAlpha = 1;
    e.endAlpha = 0;
    e.startCol = vec3(0, 0, 0.2);
    e.endCol = vec3(0, 0.6d, 1);
    e.startSize = 0;
    e.endSize = 1;
    e.boom(500);

    alive = false;
}

void Actor::collided(Actor* b)
{
    return;
}

bool Actor::collidedWithGround()
{
    return false;
}
