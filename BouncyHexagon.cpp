#include "BouncyHexagon.h"
#include "GameScene.h"

BouncyHexagon::BouncyHexagon(GameScene *sc, vec3 pos, bool movable, bool rotable, bool dest) : Hexagon(sc, pos, movable, rotable, dest)
{

}

void BouncyHexagon::update()
{

}

void BouncyHexagon::render()
{
    Hexagon::render();
}
