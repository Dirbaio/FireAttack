#include "Hexagon.h"
#include "util.h"
#include "GameScene.h"

Hexagon::Hexagon(GameScene* sc) : Actor(sc)
{
    rot = 0;
}

void Hexagon::update()
{
    rot += dt;
}

void Hexagon::render()
{
    glPushMatrix();

    float x, y, z, sx, sy, sz;
    sx = sz = 1;
    sy = 0.4;
    x = z = 0;
    y = 10;

    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    glRotatef(toDeg(rot), 0, 1, 0);
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
