#ifndef WATERHEXAGON_H
#define WATERHEXAGON_H

#include "Hexagon.h"

class GameScene;

class WaterHexagon : public Hexagon
{

public:
    WaterHexagon(GameScene* sc, vec3 pos, bool movable, bool rotable, bool dest, float r, int l);

    virtual void update();
    virtual void render();
};

#endif // WATERHEXAGON_H
