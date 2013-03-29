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
    int life, max_life;
    float regen_time_window, regen_time;

    void die();
    bool dying, dead;
    float dying_time, dying_time_window;

public:
    Hexagon(GameScene* sc, vec3 pos, bool movable, bool rotable, bool destructible, float reg, int l);

    virtual void update();
    virtual void render();
    virtual void collided(Actor *b);

    void swapRotable();
};

#endif // HEXAGON_H
