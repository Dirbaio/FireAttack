#include "GameScene.h"
#include "BoxActor.h"
#include "GroundActor.h"
#include "PlayerActor.h"
#include "FireActor.h"
#include "WallActor.h"
#include "SolidHexagon.h"
#include "BouncyHexagon.h"
#include "WaterHexagon.h"
#include "FloatingHexagon.h"
#include "ShooterEnemy.h"
#include "StandardEnemy.h"
#include "ModelActor.h"
#include "WaterPlane.h"
GameScene::GameScene()
{
    /*for(int i = 0; i < 2; i++)
        actors.push_back(new ShooterEnemy(this, 1.0, vec3(frand(10.0), 15.0, 0), vec3(0,1,0)));*/
    actors.push_back(new PlayerActor(this));
    actors.push_back(new WaterPlane(this));
    for (int k = -3; k < 0; k++) actors.push_back(new FloatingHexagon(this, vec3(k*2, 10, 0), (k%2)==0, false, true, 5.0, 4));
    for (int k = 0; k < 3; k++) actors.push_back(new BouncyHexagon(this, vec3(k*2, k*2+5, 0), false, false, true, 2.0, 2));
    for (int k = 3; k < 6; k++) actors.push_back(new WaterHexagon(this, vec3(k*2, 10, 0), true, true, false, 2.0, 5));
    for (int k = 6; k < 9; k++) actors.push_back(new SolidHexagon(this, vec3(k*2, 10, 0), (k%2)==0, false, true, 5.0, 10));
    for (int k = -10; k < 11; k++) actors.push_back(new SolidHexagon(this, vec3(k*2, 1, 0), false, false, true, 3.0, 10));
//    actors.push_back(new ModelActor(this, "test.obj"));
//    actors.push_back(new WallActor(this, -10, -1));
//    actors.push_back(new WallActor(this, 10, 1));

    deadTimer = 6;
    spawnTimer = 1;

    song1 = loadSound("game1.wav");
    song2 = loadSound("game2.wav");
    song.setBuffer(*song1);
    song.setLoop(true);
//    song.play();
    awesome = false;
}

void GameScene::goAwesome()
{
    if(awesome) return;
    awesome = true;
    song.setBuffer(*song2);
    song.setLoop(true);
//    song.play();
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
//            song.play();
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
//        if(actors.size() < 20)
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
}
