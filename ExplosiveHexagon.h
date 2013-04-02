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
    ExplosiveHexagon(GameScene* sc, vec3 pos, bool movable, bool rotable, bool dest, float r, int l);

    virtual void update();
    virtual void render();

    virtual void collided(Actor *b);
};

#endif // EXPLOSIVEHEXAGON_H
