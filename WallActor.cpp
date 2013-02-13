#include "WallActor.h"

//Simplest actor ever!
WallActor::WallActor(Scene* sc, float x, float v) : Actor(sc)
{
    p.x = x;
    p.y = 8;

    ParticleEmitter pe (this);
    pe.period = 1.0f/100.0f;
    pe.startSize = 0;
    pe.endSize = 1;
    pe.randPos = RandomVec(0, 8, 0);
    pe.startCol = vec3(0, 0.3, 1);
    pe.endCol = vec3(1, 0.3, 0);
    pe.v = vec3(0, -1, 0);
    addEmitter(pe);
}
