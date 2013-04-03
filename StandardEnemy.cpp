#include "StandardEnemy.h"
#include "GameScene.h"
#include "util.h"

StandardEnemy::StandardEnemy(GameScene* sc, vec3 pos) : Enemy(sc, 1.0, pos, vec3(0.0, 0.3, 1.0))
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
