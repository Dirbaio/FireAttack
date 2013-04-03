#include "StickyEnemy.h"
#include "GameScene.h"
#include "util.h"

StickyEnemy::StickyEnemy(GameScene* sc, vec3 pos) : Enemy(sc, 1.0, pos, vec3(1.0, 0.2, 0.6))
{
    body->SetGravityScale(0);
    this->speed = 20.0;
}

void StickyEnemy::update()
{
    Enemy::update();

    PlayerActor* nearPlayer = gsc->GetNearestPlayerInView(p);
    if (nearPlayer == NULL)
        body->ApplyForceToCenter(b2Vec2(rand()%int(4*speed)-2*speed, rand()%int(4*speed)-2*speed));
    else
    {
        vec2 normalized = vec2(nearPlayer->p.x - p.x, nearPlayer->p.y - p.y);
        normalize(normalized);
        body->ApplyForceToCenter(b2Vec2(normalized.x*speed, normalized.y*speed));
    }
}

void StickyEnemy::render()
{
    Enemy::render();
}
