#include "Enemy.h"

Enemy::Enemy(GameScene* sc, float size, vec3 pos) : Actor(sc)
{
    this->gsc = sc;
    this->player = sc->getPlayer();

    p.x = pos.x;
    p.y = pos.y;
    this->size = size;
}

void Enemy::update()
{
    player = gsc->getPlayer();
    if(!player) return;
}

void Enemy::render()
{

}
