#ifndef GROUNDACTOR_H
#define GROUNDACTOR_H

#include "Actor.h"

class GroundActor : public Actor
{
    public:

    float size;
    Texture* groundTex;
    float sx, sy, sz;

    GroundActor(Scene* sc, float x, float y, float z, float sx, float sy, float sz);
    virtual void render();
};

#endif // GROUNDACTOR_H
