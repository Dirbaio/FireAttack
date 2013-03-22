#include "StandardEnemy.h"
#include "GameScene.h"
#include "util.h"

StandardEnemy::StandardEnemy(GameScene* sc, float size, vec3 pos, vec3 col) : Enemy(sc, size, pos, col)
{

}

void StandardEnemy::update()
{
    Enemy::update();

    if(!player) return;
    body->ApplyForceToCenter(b2Vec2(player->p.x - p.x, 0));
}

void StandardEnemy::render()
{
    Enemy::render();
}
