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
#include "ExplosiveHexagon.h"
#include "ShooterEnemy.h"
#include "StandardEnemy.h"
#include "ModelActor.h"
#include "WaterPlane.h"
#include "Input.h"
#include <vector>

void configPlayers(PlayerConfig& player1, PlayerConfig& player2)
{
    player1.col1 = vec3(1.0, 1.0, 0.5);
    player1.col2 = vec3(1.0, 1.0, 1.0);
    player1.col3 = vec3(1.0, 0.0, 0.0);
    player1.col4 = vec3(1.0, 1.0, 0.0);
    player1.col5 = vec3(1.0, 0.0, 0.0);
    player1.col6 = vec3(0.0, 1.0, 0.4);

    player2.col1 = vec3(0.5, 1.0, 1.0);
    player2.col2 = vec3(0.1, 1.0, 1.0);
    player2.col3 = vec3(0.0, 0.0, 1.0);
    player2.col4 = vec3(0.0, 1.0, 1.0);
    player2.col5 = vec3(0.0, 0.0, 1.0);
    player2.col6 = vec3(0.4, 1.0, 0.0);

    player1.initPos = vec3(0, 10, 0);
    player2.initPos = vec3(4, 10, 0);

    player1.keyMap.resize(MAPPINGSIZE);
    player2.keyMap.resize(MAPPINGSIZE);

    player1.keyMap[JUMP] = sf::Keyboard::Space;
    player1.keyMap[MOVERIGHT] = sf::Keyboard::D;
    player1.keyMap[MOVELEFT] = sf::Keyboard::A;
    player1.keyMap[SHOOT] = sf::Keyboard::LShift;
    player1.keyMap[SPAWN] = sf::Keyboard::E;

    player2.keyMap[JUMP] = sf::Keyboard::RShift;
    player2.keyMap[MOVERIGHT] = sf::Keyboard::L;
    player2.keyMap[MOVELEFT] = sf::Keyboard::J;
    player2.keyMap[SHOOT] = sf::Keyboard::Return;
    player2.keyMap[SPAWN] = sf::Keyboard::O;
}

GameScene::GameScene()
{
    /*for(int i = 0; i < 2; i++)
        actors.push_back(new ShooterEnemy(this, 1.0, vec3(frand(10.0), 15.0, 0), vec3(0,1,0)));*/
    PlayerConfig player1, player2;
    configPlayers(player1, player2);
    actors.push_back(new PlayerActor(this, &player1));
    actors.push_back(new PlayerActor(this, &player2));
//    actors.push_back(new WaterPlane(this));
    for (int k = -3; k < 0; k++) actors.push_back(new FloatingHexagon(this, vec3(k*2, 10, 0), (k%2)==0, true, true, 5.0, 4));
    for (int k = 0; k < 3; k++) actors.push_back(new BouncyHexagon(this, vec3(k*2, k*2+5, 0), false, false, true, 2.0, 2));
    for (int k = 3; k < 6; k++) actors.push_back(new ExplosiveHexagon(this, vec3(k*2, 10, 0), true, true, false, 2.0, 5));
    for (int k = 6; k < 9; k++) actors.push_back(new SolidHexagon(this, vec3(k*2, 10, 0), (k%2)==0, false, true, 5.0, 10));
    for (int k = -8; k < 11; k++) actors.push_back(new SolidHexagon(this, vec3(k*2, 0.4f, 0), false, false, true, 3.0, 10));
    for (int k = 2; k < 6; k++) actors.push_back(new FloatingHexagon(this, vec3(-16+0.5*k, k, 0), true, true, true, 5.0, 4));
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

vector<PlayerActor*> GameScene::getPlayerList()
{
    vector<PlayerActor*> ret;
    for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
    {
        PlayerActor* r = dynamic_cast<PlayerActor*>(*it);
        if(r) ret.push_back(r);
    }

    return ret;
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

    vector<PlayerActor*> playerList = getPlayerList();
    if(playerList.empty())
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

    if (!playerList.empty())
    {
        vec3 minPos, maxPos;
        minPos = maxPos = playerList[0]->p;
        for (int i = 1; i < playerList.size(); i++)
        {
            PlayerActor* pl = playerList[i];
            if (pl->p.x < minPos.x) minPos = pl->p;
            if (pl->p.x > maxPos.x) maxPos = pl->p;
        }

        cameraLookAt = vec3((minPos.x+maxPos.x)/2.0, (minPos.y+maxPos.y)/2.0+1.0, 0);
        float weight = exp(-dt*6);
        cameraPos = cameraPos * weight + (vec3((minPos.x+maxPos.x)/2.0, (minPos.y+maxPos.y)/2.0 , 0) + vec3(0, 2, 7+0.25*norm(maxPos-minPos))) * (1-weight);
    }
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
