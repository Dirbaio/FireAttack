#include "GroundActor.h"
#include "Scene.h"

GroundActor::GroundActor(Scene* sc, float x, float y, float z, float sx, float sy, float sz) : Actor(sc)
{
    b2BodyDef groundBodyDef;
    b2PolygonShape groundBox;
    b2Body* groundBody;

    groundBodyDef.position.Set(x, y);
    groundBox.SetAsBox(sx, sy);
    groundBody = sc->world.CreateBody(&groundBodyDef);
    groundBody->CreateFixture(&groundBox, 0.0f);

    groundTex = loadTexture("wall.png");

    p.x = x;
    p.y = y;
    p.z = z;
    this->sx = sx;
    this->sy = sy;
    this->sz = sz;

}

void GroundActor::render()
{
    groundTex->bind();
    glColor3f(1, 1, 1);
    renderCube(p.x, p.y, p.z, sx, sy, sz, 0);
}
