#include "Enemy.h"

Enemy::Enemy(GameScene* sc) : Actor(sc)
{
    this->sc = sc;
    this->player = sc->getPlayer();
}

void Enemy::update()
{

}

void Enemy::render()
{

}
