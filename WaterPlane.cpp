#include "WaterPlane.h"

WaterPlane::WaterPlane(Scene* sc) : Actor(sc)
{
    shader = loadShader("vertex.glsl", "fragment-water.glsl");
}

void WaterPlane::render()
{
    vector<vec3> vtxArray;
    vector<vec2> texArray;

    glPushMatrix();
    glTranslatef(0, 0.01, 0);
    float s = 100;
    vtxArray.push_back(vec3(-s, 0, s));
    vtxArray.push_back(vec3(s, 0, s));
    vtxArray.push_back(vec3(s, 0, -s));
    vtxArray.push_back(vec3(-s, 0, -s));
    texArray.push_back(vec2(-s, s));
    texArray.push_back(vec2(s, s));
    texArray.push_back(vec2(s, -s));
    texArray.push_back(vec2(-s, -s));

    glNormal3f(0, 1, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, &vtxArray[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &texArray[0]);

    glDrawArrays(GL_QUADS, 0, vtxArray.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();
}

