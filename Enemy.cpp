#include "Enemy.h"
#include "PlayerActor.h"
#include "GameScene.h"
#include "WaterHexagon.h"

Enemy::Enemy(GameScene* sc, float size, vec3 pos, vec3 col) : Actor(sc)
{
    this->gsc = sc;
    this->player = sc->getPlayer();

    p.x = pos.x;
    p.y = pos.y;
    this->size = size;

    b2BodyDef bodyDef;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.type = b2_dynamicBody;
    body = sc->world.CreateBody(&bodyDef);
    body->SetLinearDamping(3);
    b2PolygonShape box;
    box.SetAsBox(size/2, size/2);
    b2FixtureDef fixture;
    fixture.density = 1.0f;
    fixture.friction = 0.1f;
    fixture.shape = &box;
    fixture.restitution = 0.6f;
    fixture.userData = this;
    body->CreateFixture(&fixture);

    ParticleEmitter pe (this);
    pe.period = 1/600.0;
    pe.startAlpha = 0.1;
    pe.randPos = RandomVec(size/4);
    pe.startSize = size/3;
    pe.endSize = size/16;
    pe.life = 0.01;
    pe.startCol = vec3(1, 1, 0.5);
    pe.endCol = vec3(1, 1, 1);
    pe.actorVelMult = 1;
    emitters.push_back(pe);

    pe.period = 1/1400.0;
    pe.randPos = RandomVec(size/2);
    pe.startSize = size/3;
    pe.endSize = size/16;
    pe.life = 0.4;
    pe.startAlpha = 0.1;
    pe.startCol = col;
    pe.endCol = vec3(0, 1, 1);
    pe.actorVelMult = -0.3;
    emitters.push_back(pe);

    body->SetUserData(this);
}

void Enemy::update()
{
    Actor::update();
    player = gsc->getPlayer();
    if(!player) return;
}

void Enemy::render()
{

}

void Enemy::collided(Actor* b)
{
    if (dynamic_cast<WaterHexagon*>(b))
    {
        explode();
    }
    if (dynamic_cast<FireActor*>(b))
    {
        explode();
    }
}
