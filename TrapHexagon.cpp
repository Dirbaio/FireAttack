#include "TrapHexagon.h"
#include "PlayerActor.h"
#include "util.h"

TrapHexagon::TrapHexagon(GameScene* sc, vec3 pos) : Hexagon(sc, pos, true, true, true, 2.0, 4)
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
    Hexagon::collided(b);
    if (dynamic_cast<PlayerActor*>(b))
    {
        activated = true;
    }
}
