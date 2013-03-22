#include "SolidHexagon.h"
#include "GameScene.h"

SolidHexagon::SolidHexagon(GameScene *sc, vec3 pos, bool movable, bool rotable, bool dest, float r, int l) : Hexagon(sc, pos, movable, rotable, dest, r, l)
{

}

void SolidHexagon::update()
{
    Hexagon::update();
}

void SolidHexagon::render()
{
    Hexagon::render();
}
