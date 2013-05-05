#include "ExplosiveHexagon.h"
#include "MagmaHexagon.h"
#include "util.h"
#include "FireActor.h"
#include "PlayerActor.h"
#include "Enemy.h"
#include "GameScene.h"

ExplosiveHexagon::ExplosiveHexagon(GameScene *sc, vec3 pos) : Hexagon(sc, pos, true, true, false, 10.0, 50)
{
    exploded = false;
    explodeTime = 0.0;
    explodeTimeMax = 2.0;
    parpadeoTime = 0.0;
    parpadeoTimeMax = 0.4;
    hexColor = vec3(1.0, 0.5, 0.0);
}

void ExplosiveHexagon::update()
{
    Hexagon::update();

    if (exploded)
    {
        parpadeoTime += dt;
        vec3 oldcol = hexColor;
        if (parpadeoTime <= parpadeoTimeMax)
            hexColor = vec3(1.0, 0.0, 0.0);
        else if (parpadeoTime > parpadeoTimeMax)
            hexColor = vec3(1.0, 0.5, 0.0);
        if(hexColor != oldcol)
            playSound("tnttick");
        if (parpadeoTime >= parpadeoTimeMax*2.0)
            parpadeoTime = 0.0;
        explodeTime += dt;
        if (explodeTime >= explodeTimeMax)
        {
            playSound("tnt");
            sc->makeExplosion(p, 50.0, true, true, 12.0f);
            alive = false;
        }
    }
}

void ExplosiveHexagon::render()
{
    shader->setParameter("color", hexColor);
    Hexagon::render();
}

void ExplosiveHexagon::collided(Actor* b)
{
    if (dynamic_cast<Enemy*>(b))
    {
        exploded = true;
        return;
    }
    if (dynamic_cast<FireActor*>(b))
    {
        exploded = true;
        return;
    }
    if (dynamic_cast<PlayerActor*>(b))
    {
        exploded = true;
        return;
    }
    if (dynamic_cast<MagmaHexagon*>(b))
    {
        exploded = true;
        return;
    }
}

void ExplosiveHexagon::ActivateExplosion()
{
    exploded = true;
}
