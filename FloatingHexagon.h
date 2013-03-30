#ifndef FLOATINGHEXAGON_H
#define FLOATINGHEXAGON_H

#include "Hexagon.h"

class GameScene;

class FloatingHexagon : public Hexagon
{

public:
    FloatingHexagon(GameScene* sc, vec3 pos, bool movable, bool rotable, bool dest, float r, int l);

    virtual void update();
    virtual void render();
};

#endif // FLOATINGHEXAGON_H
