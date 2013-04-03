#include "TrapHexagon.h"
#include "PlayerActor.h"
#include "util.h"

TrapHexagon::TrapHexagon(GameScene* sc, vec3 pos, bool dest, float r, int l) : Hexagon(sc, pos, true, true, dest, r, l)
{
    body->SetGravityScale(0);
    body->SetFixedRotation(true);
    body->SetLinearDamping(1000);
    trapTimer = 0.0;
    trapTimerMax = 0.4;
    activated = false;
}

void TrapHexagon::update()
{
    Hexagon::update();

    if (activated)
    {
        trapTimer += dt;
        if (trapTimer >= trapTimerMax)
        {
            body->SetLinearDamping(0);
            body->SetFixedRotation(false);
            body->SetGravityScale(1);
        }
    }
}

void TrapHexagon::render()
{
    shader->setParameter("color", vec3(0.8, 0.5, 0.5));
    Hexagon::render();
}

void TrapHexagon::collided(Actor* b)
{
    if (dynamic_cast<PlayerActor*>(b))
    {
        activated = true;
    }
}
