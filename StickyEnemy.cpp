#include "StickyEnemy.h"
#include "GameScene.h"
#include "util.h"

StickyEnemy::StickyEnemy(GameScene* sc, float size, vec3 pos, vec3 col, float speed) : Enemy(sc, size, pos, col)
{
    body->SetGravityScale(0);
    this->speed = speed;
}

void StickyEnemy::update()
{
    Enemy::update();

    PlayerActor* nearPlayer = gsc->GetNearestPlayerInView(p);
    if (nearPlayer == NULL)
        body->ApplyForceToCenter(b2Vec2(rand()%int(4*speed)-2*speed, rand()%int(4*speed)-2*speed));
    else
        body->ApplyForceToCenter(b2Vec2((nearPlayer->p.x - p.x) * speed, (nearPlayer->p.y - p.y) * speed));
}

void StickyEnemy::render()
{
    Enemy::render();
}
