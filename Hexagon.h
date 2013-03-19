#ifndef HEXAGON_H
#define HEXAGON_H

#include "Actor.h"

class GameScene;

class Hexagon : public Actor
{
private:
    float rot;
    float sx, sy, sz;
    bool destructible, movable, rotable;

public:
    Hexagon(GameScene* sc, vec3 pos, bool movable, bool rotable, bool destructible);

    virtual void update();
    virtual void render();
    void generateCollider();
};

#endif // HEXAGON_H
