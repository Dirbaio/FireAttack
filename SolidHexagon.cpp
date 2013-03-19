#include "SolidHexagon.h"
#include "GameScene.h"

SolidHexagon::SolidHexagon(GameScene *sc, vec3 pos, bool movable, bool rotable, bool dest) : Hexagon(sc, pos, movable, rotable, dest)
{

}

void SolidHexagon::update()
{

}

void SolidHexagon::render()
{
    Hexagon::render();
}
