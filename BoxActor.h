#ifndef BOXACTOR_H
#define BOXACTOR_H

#include "Actor.h"

class GameScene;

class BoxActor : public Actor
{
    public:

    float size;
    bool op;

    GameScene* gsc;
    BoxActor(GameScene* sc, bool op);
    Texture* playerTex;
    float shootTimer;

    virtual void update();
    virtual void render();
};

#endif // BOXACTOR_H
