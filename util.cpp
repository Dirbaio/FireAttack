#include "util.h"

static map<string, Texture*> imgMap;

vector<vec3> hexVert, hexNorm;
vector<vec2> hexTexCoord;

Texture* loadTexture(string path)
{
        map<string, Texture*>::iterator it;
        it = imgMap.find(path);
        if (it == imgMap.end()) {
                cerr << "* Loading image: " << path << endl;
                Texture* t = new Texture();
                t->loadFromFile(path);
                t->setSmooth(false);

                imgMap[path] = t;
                return t;
        }

        return it->second;
}

static map<string, SoundBuffer*> sndMap;

SoundBuffer* loadSound(string path)
{
        map<string, SoundBuffer*>::iterator it;
        it = sndMap.find(path);
        if (it == sndMap.end()) {
                cerr << "* Loading sound: " << path << endl;
                SoundBuffer* t = new SoundBuffer();
                t->loadFromFile(path);

                sndMap[path] = t;
                return t;
        }

        return it->second;
}

float cubeVert[24][3] = {
    {-1.0, 1.0, 1.0},{-1.0, -1.0, 1.0},{1.0, -1.0, 1.0},{1.0, 1.0, 1.0},
    {1.0, 1.0, -1.0},{1.0, -1.0, -1.0},{-1.0, -1.0, -1.0},{-1.0, 1.0, -1.0},
    {1.0, 1.0, 1.0},{1.0, -1.0, 1.0},{1.0, -1.0, -1.0},{1.0, 1.0, -1.0},
    {-1.0, 1.0, -1.0},{-1.0, -1.0, -1.0},{-1.0, -1.0, 1.0},{-1.0, 1.0, 1.0},
    {1.0, -1.0, 1.0},{-1.0, -1.0, 1.0},{-1.0, -1.0, -1.0},{1.0, -1.0, -1.0},
    {-1.0, 1.0, 1.0},{1.0, 1.0, 1.0},{1.0, 1.0, -1.0},{-1.0, 1.0, -1.0},
};

float cubeNormals[24][3] = {
    {0, 0, 1},{0, 0, 1},{0, 0, 1},{0, 0, 1},
    {0, 0, -1},{0, 0, -1},{0, 0, -1},{0, 0, -1},
    {1, 0, 0},{1, 0, 0},{1, 0, 0},{1, 0, 0},
    {-1, 0, 0},{-1, 0, 0},{-1, 0, 0},{-1, 0, 0},
    {0, -1, 0},{0, -1, 0},{0, -1, 0},{0, -1, 0},
    {0, 1, 0},{0, 1, 0},{0, 1, 0},{0, 1, 0},
};

float cubeTexcoords[24][2] = {
    {0, 0}, {0, 1}, {1, 1}, {1, 0},
    {0, 0}, {0, 1}, {1, 1}, {1, 0},
    {0, 0}, {0, 1}, {1, 1}, {1, 0},
    {0, 0}, {0, 1}, {1, 1}, {1, 0},
    {0, 0}, {0, 1}, {1, 1}, {1, 0},
    {0, 0}, {0, 1}, {1, 1}, {1, 0},
};

void renderCube(float x, float y, float z, float sx, float sy, float sz, float rot)
{
    glPushMatrix();

    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    glRotatef(toDeg(rot), 0, 0, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //Set the vertex and color pointers
    glVertexPointer(3, GL_FLOAT, 0, cubeVert);
    glNormalPointer(GL_FLOAT, 0, cubeNormals);
    glTexCoordPointer(2, GL_FLOAT, 0, cubeTexcoords);

    //Draw the cube using the previously specified vertices & colors, and the specified cube indices
    glDrawArrays(GL_QUADS, 0, 24);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();
}
