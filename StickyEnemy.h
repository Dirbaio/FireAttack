#ifndef STICKYENEMY_H
#define STICKYENEMY_H

#include "Enemy.h"

class StickyEnemy : public Enemy
{
private:
    float speed;

public:
    StickyEnemy(GameScene* sc, vec3 pos);

    virtual void update();
    virtual void render();
};

#endif // STICKYENEMY_H
