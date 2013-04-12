#include "StaticHexagon.h"
#include "GameScene.h"

StaticHexagon::StaticHexagon(GameScene* sc, vec3 pos) : Hexagon(sc, pos, false, false, true, 2.0, 15)
{

}

void StaticHexagon::update()
{
    Hexagon::update();
}

void StaticHexagon::render()
{
    shader->setParameter("color", vec3(0.2, 0.2, 0.2));
    Hexagon::render();
}
