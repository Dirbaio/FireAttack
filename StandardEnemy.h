#ifndef STANDARDENEMY_H
#define STANDARDENEMY_H

#include "Enemy.h"

class GameScene;

class StandardEnemy : public Enemy
{
private:

public:

    StandardEnemy(GameScene* sc, vec3 pos);

    virtual void update();
    virtual void render();
};


#endif // STANDARDENEMY_H
