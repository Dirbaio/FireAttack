#include "Hexagon.h"
#include "util.h"
#include "GameScene.h"

Hexagon::Hexagon(GameScene* sc) : Actor(sc)
{
    rot = 0;
    sx = sz = 1.0;
    sy = 0.4;
    p.x = 0;
    p.z = 0;
    p.y = 16;

    b2BodyDef bodyDef;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.type = b2_dynamicBody;
    body = sc->world.CreateBody(&bodyDef);
    body->SetLinearDamping(1.0f);
    b2PolygonShape box;
    box.SetAsBox(1.0, 0.4);
    b2FixtureDef fixture;
    fixture.density = 1.0f;
    fixture.friction = 0.0f;
    fixture.shape = &box;
    fixture.restitution = 0.3f;
    fixture.userData = this;
    body->SetFixedRotation(true);
    body->CreateFixture(&fixture);
    body->SetUserData(this);
}

void Hexagon::update()
{
}

void Hexagon::render()
{
    glPushMatrix();

    float x = p.x, y = p.y, z = p.z;
    sx = sz = 1;
    sy = 0.4;

    glTranslatef(x, y, z);
    glRotatef(toDeg(body->GetAngle()), 0, 0, 1);
    glRotatef(toDeg(rot), 0, 1, 0);
    glScalef(sx, sy, sz);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //Set the vertex and color pointers
    glVertexPointer(3, GL_FLOAT, 0, &hexVert[0]);
    glNormalPointer(GL_FLOAT, 0, &hexNorm[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &hexTexCoord[0]);

    //Draw the cube using the previously specified vertices & colors, and the specified cube indices
    glDrawArrays(GL_TRIANGLES, 0, 72);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
}

void Hexagon::generateCollider()
{

}
