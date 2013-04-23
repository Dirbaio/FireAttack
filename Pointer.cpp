#include "Pointer.h"

Pointer::Pointer(GameScene* gsc, PlayerActor* pl) : Actor(gsc)
{
    this->gsc = gsc;
    this->pl = pl;

    float size = 0.3;

    ParticleEmitter pe (this);
    pe.period = 1/500.0;
    pe.startAlpha = 0.1;
    pe.randPos = RandomVec(size/4);
    pe.startSize = size/3;
    pe.endSize = size/16;
    pe.life = 0.01;
    pe.startCol = pl->cfg.col1;
    pe.endCol = pl->cfg.col2;
    pe.actorVelMult = 1;
    emitters.push_back(pe);

    pe.period = 1/1000.0;
    pe.randPos = RandomVec(size/2);
    pe.startSize = size/3;
    pe.endSize = size/16;
    pe.life = 1;
    pe.startAlpha = 0.2;
    pe.startCol = pl->cfg.col3;
    pe.endCol = pl->cfg.col4;
    pe.actorVelMult = 0.9;
    emitters.push_back(pe);

    pe.period = 1/1000.0;
    pe.startAlpha = 0.2;
    pe.randPos = RandomVec(size/3);
    pe.randVel = RandomVec(size/3);
    pe.startSize = size/2;
    pe.endSize = size/10;
    pe.life = 1;
    pe.startCol = pl->cfg.col5;
    pe.endCol = pl->cfg.col6;
    pe.actorVelMult = 0.9;
    addEmitter(pe);

    inmortal = true;

}

void Pointer::update()
{
    vec2 pos(pl->input->getValue(POINTERX), pl->input->getValue(POINTERY));
    //Awful math below :S

    vec3 camVec = sc->cameraLookAt - sc->cameraPos;
    normalize(camVec);
    vec3 up2 (0, 1, 0);
    vec3 right = cross(camVec, up2);
    normalize(right);
    vec3 up = cross(right, camVec);
    normalize(up);

    vec3 camVec2 = camVec + right*pos.x + up*pos.y;
    vec3 res = sc->cameraPos;
    camVec2 *= res.z/camVec2.z;
    res -= camVec2;

    p = res;
}

bool Pointer::renderParticle(Particle &p)
{
    p.p = this->p;
    p.startCol = pl->cfg.col1;
    p.startAlpha = 1;
    p.startSize = 1;

    return true;
}
