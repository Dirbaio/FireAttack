#include "FloatingHexagon.h"
#include "GameScene.h"

FloatingHexagon::FloatingHexagon(GameScene *sc, vec3 pos, bool movable, bool rotable, bool dest, float r, int l) : Hexagon(sc, pos, movable, rotable, dest, r, l)
{

}

void FloatingHexagon::update()
{
    Hexagon::update();
}

void FloatingHexagon::render()
{
    shader->setParameter("color", vec3(0.7, 0.9, 0.9));
    Hexagon::render();
}
