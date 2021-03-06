#ifndef SOLIDHEXAGON_H
#define SOLIDHEXAGON_H

#include "Hexagon.h"

class GameScene;

class SolidHexagon : public Hexagon
{

public:
    SolidHexagon(GameScene* sc, vec3 pos);

    virtual void update();
    virtual void render();
};

#endif // SOLIDHEXAGON_H
