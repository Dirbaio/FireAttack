#include "StandardEnemy.h"
#include "GameScene.h"
#include "util.h"

StandardEnemy::StandardEnemy(GameScene* sc, float size, vec3 pos) : Enemy(sc, size, pos)
{

}

void StandardEnemy::update()
{
    Enemy::update();

    if(!pl) return;
    body->ApplyForceToCenter(b2Vec2(pl->p.x - p.x, 0));
}

void StandardEnemy::render()
{
    Enemy::render();
}
