#include "BouncyHexagon.h"
#include "GameScene.h"

BouncyHexagon::BouncyHexagon(GameScene *sc, vec3 pos) : Hexagon(sc, pos, false, false, true, 1.0, 8)
{
    body->GetFixtureList()->SetRestitution(1.4);
}

void BouncyHexagon::update()
{
    Hexagon::update();
}

void BouncyHexagon::render()
{
    shader->setParameter("color", vec3(1.0, 0.0, 0.8));
    Hexagon::render();
}
