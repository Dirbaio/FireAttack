#include "MagmaHexagon.h"
#include "GameScene.h"

MagmaHexagon::MagmaHexagon(GameScene *sc, vec3 pos) : Hexagon(sc, pos, true, true, false, 10.0, 50)
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
