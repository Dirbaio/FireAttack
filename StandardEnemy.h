#ifndef STANDARDENEMY_H
#define STANDARDENEMY_H

#include "Enemy.h"

class GameScene;

class StandardEnemy : public Enemy
{
private:

public:

    StandardEnemy(GameScene* sc, float size, vec3 pos, vec3 col);

    virtual void update();
    virtual void render();
};


#endif // STANDARDENEMY_H
