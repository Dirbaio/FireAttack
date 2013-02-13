#include "FireActor.h"
#include "util.h"
#include "Scene.h"
#include "BoxActor.h"
#include "GameScene.h"

FireActor::FireActor(vec3 pos, vec3 vel, GameScene* sc, bool green) : Actor(sc)
{
    gsc = sc;
    this->green = green;
    size = 0.3;

    ParticleEmitter pe (this);
    pe.period = 1/500.0;
    pe.startAlpha = 0.1;
    pe.randPos = RandomVec(size/4);
    pe.startSize = size/1;
    pe.endSize = size/16;
    pe.life = 0.01;
    pe.startCol = green?vec3(0, 1, 0):vec3(1, 0, 0);
    pe.endCol = vec3(1, 1, 0);
    pe.actorVelMult = 1;
    emitters.push_back(pe);

    pe.period = 1/1000.0;
    pe.randVel = RandomVec(1);
    pe.startAlpha = 0.4;
    pe.randPos = RandomVec();
    pe.startSize = 0.1;
    pe.endSize = 0.6;
    pe.life = 0.01;
    pe.startCol = vec3(1, 1, 0);
    pe.endCol = green?vec3(0, 1, 1):vec3(1, 0, 1);
    pe.actorVelMult = 0;
    emitters.push_back(pe);

    p = pos;
    v = vel;
    b2BodyDef bodyDef;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.type = b2_dynamicBody;
    body = sc->world.CreateBody(&bodyDef);
    body->SetLinearDamping(0);
    body->SetGravityScale(0);
    body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
    b2CircleShape box;
    box.m_radius = size/2;
    b2FixtureDef fixture;
    fixture.density = 1.0f;
    fixture.friction = 0.0f;
    fixture.shape = &box;
    fixture.restitution = 1;
    fixture.userData = this;
    body->CreateFixture(&fixture);

    body->SetUserData(this);
    bounceCount = 5;
}

void FireActor::update()
{
    if(p.y > 10) explode();

    p += v*dt;
    if(p.y < 0 || p.y > 20 || p.x < -10 || p.x > 10)
        explode();

    body->SetTransform(b2Vec2(p.x, p.y), 0);
}

void FireActor::render()
{
}

bool FireActor::collided(Actor *b)
{
    BoxActor* x = dynamic_cast<BoxActor*>(b);
    if(x && green)
    {
        explode();
        return false;
    }

    if(x && x->op && !gsc->awesome)
    {
        ParticleEmitter e(this);
        e.randVel = RandomVec(3, CIRCLE_XY);
        e.life = 1;
        e.startAlpha = 1;
        e.endAlpha = 0;
        e.startCol = vec3(0, 0, 1);
        e.endCol = vec3(0, 0.4, 0);
        e.startSize = 0;
        e.endSize = 0.4;
        e.boom(2000);
        e.randVel = RandomVec(2, CIRCLE_XY);
        e.life = 1;
        e.startAlpha = 1;
        e.endAlpha = 0;
        e.startCol = vec3(1, 0, 0);
        e.endCol = vec3(0, 0.4, 0);
        e.startSize = 0;
        e.endSize = 0.4;
        e.boom(2000);
        e.randVel = RandomVec(20, CIRCLE_XY);
        e.life = 1;
        e.startAlpha = 1;
        e.endAlpha = 0;
        e.startCol = vec3(1, 0, 0);
        e.endCol = vec3(0, 0.4, 0);
        e.startSize = 0.3;
        e.endSize = 0.4;
        e.boom(200);
        gsc->goAwesome();
    }

    b->explode();
    alive = false;
    return true;
}

bool FireActor::collidedWithGround()
{
    bounceCount--;
    if(bounceCount == 0)
    {
        explode();
        return true;
    }
    return false;
//    explode();
}
