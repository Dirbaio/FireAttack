#include "SolidHexagon.h"
#include "GameScene.h"

SolidHexagon::SolidHexagon(GameScene *sc, vec3 pos) : Hexagon(sc, pos, true, false, true, 2.0, 15)
{

}

void SolidHexagon::update()
{
    Hexagon::update();
}

void SolidHexagon::render()
{
    shader->setParameter("color", vec3(0.2, 0.2, 0.2));
    Hexagon::render();
}
