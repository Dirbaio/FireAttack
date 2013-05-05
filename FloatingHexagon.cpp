#include "FloatingHexagon.h"
#include "GameScene.h"

FloatingHexagon::FloatingHexagon(GameScene *sc, vec3 pos) : Hexagon(sc, pos, true, true, true, 3.0, 1)
{
//    body->SetAngularDamping(20.0f);
    body->SetLinearDamping(5.0f);
    body->GetFixtureList()->SetDensity(400.0f);
}

void FloatingHexagon::update()
{
    Hexagon::update();
    if (p.y <= 0.1)
    {
        body->ApplyForceToCenter(b2Vec2(0.0, 340.0f*(0.1-p.y)*(0.1-p.y)));
        float ang = body->GetAngle();
        ang = fmod(ang, 2*M_PI);
        if(ang < 0) ang += 2*M_PI;
        if(ang > M_PI) ang -= 2*M_PI;
        body->SetAngularVelocity(-ang*4);
    }
}

void FloatingHexagon::render()
{
    shader->setParameter("color", vec3(0.8, 0.9, 0.9));
    Hexagon::render();
}
