#include "GameScene.h"
#include "BoxActor.h"
#include "GroundActor.h"
#include "PlayerActor.h"
#include "FireActor.h"
#include "WallActor.h"

bool intersection(vec3 a, vec3 b, vec3& out)
{
    if(a.z < 0 && b.z < 0) return false;
    if(a.z > 0 && b.z > 0) return false;
    if(a.z == 0 && b.z == 0) return false;

    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dz = b.z - a.z;

    out.z = 0;
    out.x = a.x - dx/dz*a.z;
    out.y = a.y - dy/dz*a.z;

    return true;
}

GameScene::GameScene() : mdl("test.obj")
{
//    for(int i = 0; i < 10; i++)
//        actors.push_back(new BoxActor(this, false));
    actors.push_back(new PlayerActor(this));
//    actors.push_back(new WallActor(this, -10, -1));
//    actors.push_back(new WallActor(this, 10, 1));

    float sz = 20;

    vertexs.push_back(-sz); vertexs.push_back(0); vertexs.push_back(-sz);
    vertexs.push_back(sz);  vertexs.push_back(0); vertexs.push_back(-sz);
    vertexs.push_back(sz);  vertexs.push_back(0); vertexs.push_back(sz);
    vertexs.push_back(-sz); vertexs.push_back(0); vertexs.push_back(sz);

    texcoords.push_back(-sz); texcoords.push_back(-sz);
    texcoords.push_back(sz);  texcoords.push_back(-sz);
    texcoords.push_back(sz);  texcoords.push_back(sz);
    texcoords.push_back(-sz); texcoords.push_back(sz);

    float r = 1;
    float g = 1;
    float b = 1;

    for(int i = 0; i < 4; i++)
    {
        colors.push_back(r);
        colors.push_back(g);
        colors.push_back(b);
    }


    /*
    vector<b2Vec2> points2;
    points2.push_back(b2Vec2(-10, 100));
    points2.push_back(b2Vec2(-10, 0));
    points2.push_back(b2Vec2(10, 0));
    points2.push_back(b2Vec2(10, 100));

    b2BodyDef groundBodyDef;
    b2Body* groundBody;

    groundBodyDef.position.Set(0, 0);
    groundBody = world.CreateBody(&groundBodyDef);
    b2ChainShape chain;
    chain.CreateChain(&points2[0], points2.size());
    groundBody->CreateFixture(&chain, 0.0f);
*/
    for(int i = 0; i < mdl.vertexArray.size(); i += 3)
    {
        vec3 a = mdl.vertexArray[i+0];
        vec3 b = mdl.vertexArray[i+1];
        vec3 c = mdl.vertexArray[i+2];

        vector<vec3> pts;
        vec3 p;

        if(intersection(a, b, p)) pts.push_back(p);
        if(intersection(c, b, p)) pts.push_back(p);
        if(intersection(c, a, p)) pts.push_back(p);

        if(pts.size() == 2)
        {
            b2BodyDef bodyDef;
            b2Body* body;

            bodyDef.position.Set(0, 0);
            body = world.CreateBody(&bodyDef);
            b2EdgeShape edge;
            edge.Set(b2Vec2(pts[0].x, pts[0].y), b2Vec2(pts[1].x, pts[1].y));
            body->CreateFixture(&edge, 0.0f);

        }
    }

    deadTimer = 6;
    spawnTimer = 1;

    song1 = loadSound("game1.wav");
    song2 = loadSound("game2.wav");
    song.setBuffer(*song1);
    song.setLoop(true);
    song.play();
    awesome = false;
}

void GameScene::goAwesome()
{
    if(awesome) return;
    awesome = true;
    song.setBuffer(*song2);
    song.setLoop(true);
    song.play();
}

PlayerActor* GameScene::getPlayer()
{
    for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
    {
        PlayerActor* r = dynamic_cast<PlayerActor*>(*it);
        if(r) return r;
    }

    return NULL;
}

void GameScene::update()
{
    Scene::update();
    if(awesome)
    {
        float pos = song.getPlayingOffset().asSeconds();
        pos *= 132.0f / 60.0f;
        pos = fmod(pos, 1)*3 - 2;
        particlePosMult = 1 + pos;
    }

    PlayerActor* pl = getPlayer();
    if(!pl)
    {
        if(deadTimer == 6)
        {
            song.stop();
            song.setBuffer(*song2);
            song.setPlayingOffset(seconds(60+30.9df));
            song.play();
        }
        deadTimer -= dt;
    }
    if(deadTimer <= 0 && !nextScene)
        nextScene = new GameScene();
/*
    spawnTimer -= dt;
    if(spawnTimer < 0)
    {
        spawnTimer = 1 + frand(1);
        if(actors.size() < 20)
            actors.push_back(new BoxActor(this, prand(awesome?0.9:0.1)));

        if(awesome && prand(0.05))
            for(int i = 0; i < 3; i++)
                actors.push_back(new BoxActor(this, true));
    }*/
}

void GameScene::render()
{
    Scene::render();
/*    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // [!] Juanking SFML

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //Set the vertex and color pointers
    glVertexPointer(3, GL_FLOAT, 0, &vertexs[0]);
    glColorPointer(3, GL_FLOAT, 0, &colors[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);

    //Draw the cube using the previously specified vertices & colors, and the specified cube indices
    glDrawArrays(GL_QUADS, 0, vertexs.size()/3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
*/
    mdl.draw();
}

void GameScene::BeginContact(b2Contact *contact)
{
    Actor* a = (Actor*) contact->GetFixtureA()->GetBody()->GetUserData();
    Actor* b = (Actor*) contact->GetFixtureB()->GetBody()->GetUserData();

    if(a && b)
    {
        if(!a->collided(b))
            b->collided(a);
    }
    else if(a)
        a->collidedWithGround();
    else if(b)
        b->collidedWithGround();
}
