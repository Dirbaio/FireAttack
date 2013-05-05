#include "util.h"
#include<list>

RenderWindow* app;

vector<vec3> hexVert, hexNorm;
vector<vec2> hexTexCoord;

Font font;
float dt;
float tim = 0;

static map<string, Texture*> imgMap;

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

SoundBuffer* loadSound(string name)
{
    map<string, SoundBuffer*>::iterator it;
    it = sndMap.find(name);
    if (it == sndMap.end()) {
        cerr << "* Loading sound: " << name << endl;
        SoundBuffer* t = new SoundBuffer();
        if(!t->loadFromFile("sound/"+name+".wav"))
        {
            cerr<<"Cant load sound "<<name<<endl;
            app->close();
        }

        sndMap[name] = t;
        return t;
    }

    return it->second;
}

static map<pair<string, string>, Shader*> shaderMap;

Shader* loadShader(string vertex, string fragment)
{
    pair<string, string> path (vertex, fragment);
    map<pair<string, string>, Shader*>::iterator it;
    it = shaderMap.find(path);
    if (it == shaderMap.end()) {
        cerr << "* Loading shader: " << vertex << " " << fragment << endl;
        Shader* t = new Shader();
        if (!t->loadFromFile("shaders/"+vertex, "shaders/"+fragment))
            exit(1);

        shaderMap[path] = t;
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



void makeHexagon()
{
    float DIS = 1.154700538;
    float COS = 0.577350269;
    float SIN = 0.866025404;
    float x[] = {-1.0, 0.0, 1.0, 1.0, 0.0, -1.0, -1.0};
    float y[] = {1.0, -1.0};
    float z[] = {COS, DIS, COS, -COS, -DIS, -COS, COS};
    float nx[] = {-0.5, 0.5, 1.0, 0.5, -0.5, -1.0};
    float nz[] = {SIN, SIN, 0.0, -SIN, -SIN, 0.0};
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (i == 0)
            {
                hexVert.push_back(vec3(x[j], y[i], z[j]));
                hexVert.push_back(vec3(x[j+1], y[i], z[j+1]));
                hexVert.push_back(vec3(0.0, y[i], 0.0));
                for (int k = 0; k < 3; k++) hexNorm.push_back(vec3(0.0, 1.0, 0.0));
                hexTexCoord.push_back(vec2((6.0-j)/6.0, 1.0/3.0));
                hexTexCoord.push_back(vec2((5.0-j)/6.0, 1.0/3.0));
                hexTexCoord.push_back(vec2((5.5-j)/6.0, 0.0));

                hexVert.push_back(vec3(x[j], y[0], z[j]));
                hexVert.push_back(vec3(x[j+1], y[1], z[j+1]));
                hexVert.push_back(vec3(x[j+1], y[0], z[j+1]));
                for (int k = 0; k < 3; k++) hexNorm.push_back(vec3(nx[j], 0.0, nz[j]));
                hexTexCoord.push_back(vec2((6.0-j)/6.0, 1.0/3.0));
                hexTexCoord.push_back(vec2((5.0-j)/6.0, 2.0/3.0));
                hexTexCoord.push_back(vec2((5.0-j)/6.0, 1.0/3.0));
            }
            else
            {
                hexVert.push_back(vec3(x[j+1], y[i], z[j+1]));
                hexVert.push_back(vec3(x[j], y[i], z[j]));
                hexVert.push_back(vec3(0.0, y[i], 0.0));
                for (int k = 0; k < 3; k++) hexNorm.push_back(vec3(0.0, -1.0, 0.0));
                hexTexCoord.push_back(vec2((5.0-j)/6.0, 2.0/3.0));
                hexTexCoord.push_back(vec2((6.0-j)/6.0, 2.0/3.0));
                hexTexCoord.push_back(vec2((5.5-j)/6.0, 1.0));

                hexVert.push_back(vec3(x[j+1], y[1], z[j+1]));
                hexVert.push_back(vec3(x[j], y[0], z[j]));
                hexVert.push_back(vec3(x[j], y[1], z[j]));
                for (int k = 0; k < 3; k++) hexNorm.push_back(vec3(nx[j], 0.0, nz[j]));
                hexTexCoord.push_back(vec2((5.0-j)/6.0, 2.0/3.0));
                hexTexCoord.push_back(vec2((6.0-j)/6.0, 1.0/3.0));
                hexTexCoord.push_back(vec2((6.0-j)/6.0, 2.0/3.0));
            }
        }
    }
    for(int i = 0; i < hexVert.size(); i++)
    {
        hexNorm[i] = hexNorm[i]*0.8f+hexVert[i]*0.2f;
        normalize(hexNorm[i]);
    }
}

Music music;
string playingMusic;

void playMusic(string name)
{
    if(name == playingMusic)
        return;
    if(playingMusic != "")
        stopMusic();

    if(!music.openFromFile("sound/music/"+name+".wav"))
    {
        cerr<<"Cant load music "<<name<<endl;
        exit(1);
    }
    music.setLoop(true);
    music.play();
    playingMusic = true;
}

void stopMusic()
{
    if(playingMusic == "")
        return;

    playingMusic = "";
    music.stop();
}

list<Sound> sounds;

void playSound(string name)
{
    for(list<Sound>::iterator it = sounds.begin(); it != sounds.end(); )
    {
        if(!it->getStatus() != sf::Sound::Playing)
        {
            list<Sound>::iterator it2 = it;
            it2++;
            sounds.erase(it);
            it = it2;
        }
        else
            it++;
    }

    //Avoid sound spam.
    if(sounds.size() > 10) return;

    Sound s;
    s.setBuffer(*(loadSound(name)));
    sounds.push_back(s);
    sounds.back().play();
}
