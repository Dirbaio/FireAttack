#ifndef TRAPHEXAGON_H
#define TRAPHEXAGON_H

#include "Hexagon.h"

class GameScene;

class TrapHexagon : public Hexagon
{
private:
    bool activated;
    float trapTimer, trapTimerMax;

public:
    TrapHexagon(GameScene* sc, vec3 pos);

    virtual void update();
    virtual void render();

    virtual void collided(Actor* b);
};

#endif // TRAPHEXAGON_H
