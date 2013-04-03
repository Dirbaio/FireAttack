#include "FireActor.h"
#include "util.h"
#include "Scene.h"
#include "GameScene.h"
#include "BouncyHexagon.h"
#include "SolidHexagon.h"
#include "WaterHexagon.h"
#include "Enemy.h"

FireActor::FireActor(vec3 pos, vec3 vel, GameScene* sc, BulletConfig* config) : Actor(sc)
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
    pe.startCol = config->col1;
    pe.endCol = config->col2;
    pe.actorVelMult = 1;
    emitters.push_back(pe);

    pe.period = 1/1000.0;
    pe.randVel = RandomVec(1);
    pe.startAlpha = 0.4;
    pe.randPos = RandomVec();
    pe.startSize = 0.1;
    pe.endSize = 0.6;
    pe.life = 0.01;
    pe.startCol = config->col3;
    pe.endCol = config->col4;
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
    Actor::update();
    p += v*dt;
    if(p.y > 1000 || p.x < -2000 || p.x > 2000)
        explode();

    body->SetTransform(b2Vec2(p.x, p.y), 0);
}

void FireActor::render()
{
}

void FireActor::collided(Actor *b)
{

    if(dynamic_cast<Hexagon*>(b))
    {
        if (dynamic_cast<WaterHexagon*>(b))
        {
            explode();
            return;
        }
        bounceCount--;
        if(bounceCount == 0)
        {
            explode();
        }
        return;
    }

    if(dynamic_cast<Enemy*>(b))
    {
        explode();
    }

    if (dynamic_cast<PlayerActor*>(b))
    {
        explode();
    }

    if (dynamic_cast<FireActor*>(b))
    {
        explode();
    }
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
}
