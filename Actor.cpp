#include "Actor.h"
#include "Scene.h"
#include "FloatingHexagon.h"
#include "PlayerActor.h"

Actor::Actor(Scene* sc)
{
	this->sc = sc;
	alive = true;
//	emitters.push_back(ParticleEmitter(this));
    particlePosMult = 1;
    body = NULL;
    shader = NULL;
    inmortal = false;
}
Actor::~Actor() {}

void Actor::update()
{
    if (p.y <= -0.1 && dynamic_cast<FloatingHexagon*>(this) == NULL)
        explodeWater();
}
void Actor::render() {}
bool Actor::renderParticle(Particle &p) { return false; }

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

    /*
    PlayerActor* pl = dynamic_cast< PlayerActor* const> (this);
    if(pl)
    {
        v.x = 10;
        v.y = -5;
        p.x = 0;
        p.y = 10;
    }*/

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
    e.boom(1500);

    sc->makeExplosion(p, 15.0, false, false, 3.0);

    alive = false;
}

void Actor::explodeWater()
{
    playSound("boom");
    ParticleEmitter e(this);
    e.a = vec3(0, -7, 0);
    e.lightPermil = 0;
    e.randVel = RandomVec(2, CIRCLE_XZ);
    e.randCol = RandomVec(0, 0.2, 0.3);
    e.randVel.lol = 2;
    e.v = vec3(0, 3, 0);
    e.life = 1;
    e.startAlpha = 1;
    e.endAlpha = 1;
    e.startCol = vec3(0.1, 0.2, 0.7);
    e.endCol = vec3(0, 0.1, 0.2);
    e.startSize = 0.7;
    e.endSize = 0;
    e.boom(1000);

    e.randVel = RandomVec(0.3, 5, 0.4);
    e.boom(1000);
    e.a = vec3(0, 0, 0);
    e.randVel = RandomVec(6, CIRCLE_XZ);
    e.v = vec3(0, 1, 0);
    e.boom(1000);

    sc->makeExplosion(p, 25.0, false, false, 5.0);

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
