#ifndef MAGMAHEXAGON_H
#define MAGMAHEXAGON_H

#include "Hexagon.h"

class GameScene;

class MagmaHexagon : public Hexagon
{

public:
    MagmaHexagon(GameScene* sc, vec3 pos, bool movable, bool rotable, bool dest, float r, int l);

    virtual void update();
    virtual void render();
};

#endif // MAGMAHEXAGON_H
