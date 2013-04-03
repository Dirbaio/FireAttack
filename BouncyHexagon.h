#ifndef BOUNCYHEXAGON_H
#define BOUNCYHEXAGON_H

#include "Hexagon.h"

class GameScene;

class BouncyHexagon : public Hexagon
{

public:
    BouncyHexagon(GameScene* sc, vec3 pos);

    virtual void update();
    virtual void render();
};


#endif // BOUNCYHEXAGON_H
