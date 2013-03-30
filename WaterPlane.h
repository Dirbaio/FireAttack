#ifndef WATERPLANE_H
#define WATERPLANE_H

#include "Actor.h"
#include "util.h"

class WaterPlane: public Actor
{
public:
    WaterPlane(Scene* sc);
    virtual void render();
};

#endif // WATERPLANE_H
