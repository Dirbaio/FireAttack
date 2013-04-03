#include "MagmaHexagon.h"
#include "GameScene.h"

MagmaHexagon::MagmaHexagon(GameScene *sc, vec3 pos, bool movable, bool rotable, bool dest, float r, int l) : Hexagon(sc, pos, movable, rotable, dest, r, l)
{
    body->GetFixtureList()->SetFriction(0.1);
}

void MagmaHexagon::update()
{
    Hexagon::update();
}

void MagmaHexagon::render()
{
    shader->setParameter("color", vec3(1.0, 0.0, 0.0));
    Hexagon::render();
}
