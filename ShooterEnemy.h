#ifndef SHOOTERENEMY_H
#define SHOOTERENEMY_H

#include "Enemy.h"

class GameScene;

class ShooterEnemy : public En
		emy
{
private:
    float shootTimer;

public:

    ShooterEnemy(GameScene* sc, float size, vec3 pos, vec3 col);

    virtual void update();
    virtual void render();
};

#endif // SHOOTERENEMY_H
