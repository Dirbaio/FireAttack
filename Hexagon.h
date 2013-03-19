#ifndef HEXAGON_H
#define HEXAGON_H

#include "Actor.h"

class GameScene;

class Hexagon : public Actor
{
private:
    int type;
    float rot;

public:
    Hexagon(GameScene* sc);

    virtual void update();
    virtual void render();
    void generateCollider();
};

#endif // HEXAGON_H
