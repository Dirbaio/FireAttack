#ifndef EXPLOSIVEHEXAGON_H
#define EXPLOSIVEHEXAGON_H

#include "Hexagon.h"

class GameScene;

class ExplosiveHexagon : public Hexagon
{
public:
    ExplosiveHexagon(GameScene* sc, vec3 pos, bool movable, bool rotable, bool dest, float r, int l);

    virtual void update();
    virtual void render();
};

#endif // EXPLOSIVEHEXAGON_H
