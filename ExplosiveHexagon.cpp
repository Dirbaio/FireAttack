#include "ExplosiveHexagon.h"
#include "GameScene.h"

ExplosiveHexagon::ExplosiveHexagon(GameScene *sc, vec3 pos, bool movable, bool rotable, bool dest, float r, int l) : Hexagon(sc, pos, movable, rotable, dest, r, l)
{

}

void ExplosiveHexagon::update()
{
    Hexagon::update();
}

void ExplosiveHexagon::render()
{
    shader->setParameter("color", vec3(1.0, 0.5, 0.0));
    Hexagon::render();
}
