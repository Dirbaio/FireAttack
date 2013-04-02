#include "FloatingHexagon.h"
#include "GameScene.h"

FloatingHexagon::FloatingHexagon(GameScene *sc, vec3 pos, bool movable, bool rotable, bool dest, float r, int l) : Hexagon(sc, pos, movable, rotable, dest, r, l)
{
}

void FloatingHexagon::update()
{
    Hexagon::update();
    if (p.y <= 0.1)
    {
        body->ApplyForceToCenter(b2Vec2(0.0, 34.0*60.0*dt));
        float ang = body->GetAngle();
        ang = fmod(ang, 2*M_PI);
        if(ang < 0) ang += 2*M_PI;
        if(ang > M_PI) ang -= 2*M_PI;
        body->SetAngularVelocity(-ang);
    }
}

void FloatingHexagon::render()
{
    shader->setParameter("color", vec3(0.7, 0.9, 0.9));
    Hexagon::render();
}
