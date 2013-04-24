#include "GameScene.h"
#include "BoxActor.h"
#include "GroundActor.h"
#include "PlayerActor.h"
#include "FireActor.h"
#include "WallActor.h"

GameScene::GameScene()
{
    for(int i = 0; i < 10; i++)
        actors.push_back(new BoxActor(this, false));
    actors.push_back(new PlayerActor(this));
    actors.push_back(new WallActor(this, -10, -1));
    actors.push_back(new WallActor(this, 10, 1));
    groundTex = loadTexture("wall.png");

    vector<b2Vec2> points;
    points.push_back(b2Vec2(-14, 0));
    points.push_back(b2Vec2(-10, 0));
    points.push_back(b2Vec2(10, 0));
    points.push_back(b2Vec2(14, 0));

    float thick = 20;
    float texsize = 15;
    float texmargin = (thick-texsize)/2;
    float texloop = texsize; //Texture is 2x wide.
    float texpos = 0;

    for(int i = 0; i < points.size()-1; i++)
    {
        b2Vec2 a = points[i];
        b2Vec2 b = points[i+1];
        vertexs.push_back(a.x);
        vertexs.push_back(a.y);
        vertexs.push_back(-thick);
        vertexs.push_back(a.x);
        vertexs.push_back(a.y);
        vertexs.push_back(thick);
        vertexs.push_back(b.x);
        vertexs.push_back(b.y);
        vertexs.push_back(thick);
        vertexs.push_back(b.x);
        vertexs.push_back(b.y);
        vertexs.push_back(-thick);

        if(i == 0)
            for(int i = 0; i < 6; i++)
                colors.push_back(0);
        else
            for(int i = 0; i < 6; i++)
                colors.push_back(1);
        if(i == points.size()-2)
            for(int i = 0; i < 6; i++)
                colors.push_back(0);
        else
            for(int i = 0; i < 6; i++)
                colors.push_back(1);

        texcoords.push_back(texpos);
        texcoords.push_back(1+texmargin);
        texcoords.push_back(texpos);
        texcoords.push_back(-texmargin);
        texpos += norm(vec3(a.x-b.x, a.y-b.y, 0)) / texloop;
        texcoords.push_back(texpos);
        texcoords.push_back(-texmargin);
        texcoords.push_back(texpos);
        texcoords.push_back(1+texmargin);
    }


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

    groundTex = loadTexture("wall.png");

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

    spawnTimer -= dt;
    if(spawnTimer < 0)
    {
        spawnTimer = 1 + frand(1);
        if(actors.size() < 20)
            actors.push_back(new BoxActor(this, prand(awesome?0.9:0.1)));

        if(awesome && prand(0.05))
            for(int i = 0; i < 3; i++)
                actors.push_back(new BoxActor(this, true));
    }
}

void GameScene::render()
{
    Scene::render();
    Texture::bind(groundTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // [!] Juanking SFML

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
