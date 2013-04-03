#ifndef EXPLOSIVEHEXAGON_H
#define EXPLOSIVEHEXAGON_H

#include "Hexagon.h"

class GameScene;

class ExplosiveHexagon : public Hexagon
{
private:
    bool exploded;
    float explodeTime, explodeTimeMax;
    float parpadeoTime, parpadeoTimeMax;
    vec3 hexColor;

public:
    ExplosiveHexagon(GameScene* sc, vec3 pos);

    virtual void update();
    virtual void render();

    virtual void collided(Actor *b);

    void ActivateExplosion();
};

#endif // EXPLOSIVEHEXAGON_H
