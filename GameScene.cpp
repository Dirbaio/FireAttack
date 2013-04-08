#include "GameScene.h"
#include "PlayerActor.h"
#include "FireActor.h"
#include "ModelActor.h"
#include "Input.h"
#include "Level.h"
#include <vector>

GameScene::GameScene(GameMode mode)
{
    numPlayers = 1;
    gameMode = mode;

    list<Asset> assetList;

    for(int i = 0; i < 2; i++)
        assetList.push_back(Asset(ENEMSTICKY, vec3(i*2, 25.0, 0.0)));

    if (numPlayers >= 1)
        assetList.push_back(Asset(PLAYER1, vec3(0.0, 0.0, 0.0)));
    if (numPlayers >= 2)
        assetList.push_back(Asset(PLAYER2, vec3(0.0, 0.0, 0.0)));

    for (int k = -3; k < 0; k++) assetList.push_back(Asset(HEXTRAP, vec3(k*3, 8, 0)));
    for (int k = 0; k < 3; k++) assetList.push_back(Asset(HEXBOUNCY, vec3(k*2, k*2+5, 0)));
    for (int k = 3; k < 6; k++) assetList.push_back(Asset(HEXEXPLOSIVE, vec3(k*2, 10, 0)));
    for (int k = 6; k < 9; k++) assetList.push_back(Asset(HEXSOLID, vec3(k*2, 10, 0)));
    for (int k = -8; k < 20; k++) assetList.push_back(Asset(HEXSTATIC, vec3(k*2, 0.4f, 0)));
    for (int k = 2; k < 6; k++) assetList.push_back(Asset(HEXFLOATING, vec3(-16+0.5*k, k, 0)));

    Level* lvl = new Level(this, "Test", "Test.lvl", assetList);
    lvl->Build();

    deadTimer = 6;
    spawnTimer = 1;

    song1 = loadSound("game1.wav");
    song2 = loadSound("game2.wav");
    song.setBuffer(*song1);
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
        nextScene = new GameScene(FREEMODE);

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

PlayerActor* GameScene::GetNearestPlayerInView(vec3 pos)
{
    float bestDist = -1;
    PlayerActor* best = NULL;
    for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); it++)
    {
        if (dynamic_cast<PlayerActor*>(*it))
        {
            float dist = norm(pos-(*it)->p);
            if (bestDist != -1 && dist > bestDist)
                continue;

            float rayFract = GetRayCastDistance(b2Vec2(pos.x, pos.y), b2Vec2((*it)->p.x, (*it)->p.y));
            if (rayFract >= 1 - 0.5/dist - 0.1 && (dist < bestDist || bestDist == -1))
            {
                best = dynamic_cast<PlayerActor*>(*it);
                bestDist = dist;
            }
        }
    }
    return best;
}
