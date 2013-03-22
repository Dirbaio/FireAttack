#include "BouncyHexagon.h"
#include "GameScene.h"

BouncyHexagon::BouncyHexagon(GameScene *sc, vec3 pos, bool movable, bool rotable, bool dest, float r, int l) : Hexagon(sc, pos, movable, rotable, dest, r, l)
{

}

void BouncyHexagon::update()
{
    Hexagon::update();
}

void BouncyHexagon::render()
{
    Hexagon::render();
}
