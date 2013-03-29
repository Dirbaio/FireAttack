#include "WaterHexagon.h"
#include "GameScene.h"

WaterHexagon::WaterHexagon(GameScene *sc, vec3 pos, bool movable, bool rotable, bool dest, float r, int l) : Hexagon(sc, pos, movable, rotable, dest, r, l)
{
    body->GetFixtureList()->SetFriction(0.1);
}

void WaterHexagon::update()
{
    Hexagon::update();
}

void WaterHexagon::render()
{
    shader->setParameter("color", vec3(0.0, 1.0, 1.0));
    Hexagon::render();
}
