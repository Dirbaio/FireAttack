#ifndef STICKYENEMY_H
#define STICKYENEMY_H

#include "Enemy.h"

class StickyEnemy : public Enemy
{
private:
    float speed;

public:
    StickyEnemy(GameScene* sc, float size, vec3 pos, vec3 col, float speed);

    virtual void update();
    virtual void render();
};

#endif // STICKYENEMY_H
