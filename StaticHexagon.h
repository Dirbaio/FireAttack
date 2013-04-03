#ifndef STATICHEXAGON_H
#define STATICHEXAGON_H

#include "Hexagon.h"

class GameScene;

class StaticHexagon : public Hexagon
{
public:
    StaticHexagon(GameScene *sc, vec3 pos);
};

#endif // STATICHEXAGON_H
