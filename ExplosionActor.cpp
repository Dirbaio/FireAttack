#include "ExplosionActor.h"
#include "ExplosiveHexagon.h"
#include "Hexagon.h"

ExplosionActor::ExplosionActor(Scene* sc, vec3 pos, float force, bool explodes, bool destroys, float inc) : Actor(sc)
{
    p.x = pos.x;
    p.y = pos.y;
    p.z = pos.z;
    oldp = p;

    increase = inc;

    if (explodes)
    {
        ParticleEmitter e(this);
        e.randVel = RandomVec(inc*0.75);
        e.life = 1.3;
        e.startAlpha = 1;
        e.endAlpha = 0;
        e.startCol = vec3(1, 0, 0);
        e.endCol = vec3(1, 0.4, 0);
        e.startSize = 0;
        e.endSize = 1;
        e.boom(800);
        e.randVel = RandomVec(inc*0.75);
        e.life = 1.3;
        e.startAlpha = 1;
        e.endAlpha = 0;
        e.startCol = vec3(1, 0, 0);
        e.endCol = vec3(1, 0, 1);
        e.startSize = 0;
        e.endSize = 1;
        e.boom(800);
/*
        e.randVel = RandomVec(0, 6, 0);
        e.randPos = RandomVec(0, 1, 0);
        e.life = 1.3;
        e.startAlpha = 1;
        e.endAlpha = 0;
        e.startCol = vec3(1, 1, 1);
        e.endCol = vec3(1, 0.6d, 0);
        e.startSize = 0;
        e.endSize = 1;
        e.boom(1000);*/
    }

    this->destroys = destroys;
    timer = 0.0;
    this->force = force;

    radio = 0.0;

}

void ExplosionActor::update()
{
    timer += dt;
    if (timer >= 1.3)
    {
        alive = false;
        return;
    }

    radio += dt*increase;

    for(list<Actor*>::iterator it = sc->actors.begin(); it != sc->actors.end(); ++it)
    {
        if (dynamic_cast<ExplosionActor*>(*it))
            continue;
        vec3 dir = (*it)->p - p;
        if (isZero(dir)) continue;
        float ratio = force/norm(dir);
        if (ratio <= 10.5-radio) continue;
        normalize(dir);
        vec2 normal2 = vec2(dir.x, dir.y);
        if (dynamic_cast<Hexagon*>(*it))
        {
            if (!dynamic_cast<Hexagon*>(*it)->isMovable() && destroys && ratio >= 20.5-radio)
            {
               (*it)->explode();
               continue;
            }
            if (dynamic_cast<ExplosiveHexagon*>(*it) && ratio >= 18.0-radio)
                dynamic_cast<ExplosiveHexagon*>(*it)->ActivateExplosion();
        }

        (*it)->body->ApplyForceToCenter(b2Vec2(normal2.x*ratio*2.0, normal2.y*ratio*2.0));
    }
}
