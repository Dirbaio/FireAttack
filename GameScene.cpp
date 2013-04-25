#include "GameScene.h"
#include "PlayerActor.h"
#include "FireActor.h"
#include "ModelActor.h"
#include "Input.h"
#include <vector>

#include "StaticHexagon.h"
#include "SolidHexagon.h"
#include "TrapHexagon.h"
#include "BouncyHexagon.h"
#include "ShooterEnemy.h"
#include "ExplosiveHexagon.h"

GameScene::GameScene(GameMode mode, int numPlayers)
{
    this->numPlayers = numPlayers;
    gameMode = mode;


    for(int i = 0; i < numPlayers; i++)
        actors.push_front(new PlayerActor(this, playerConfigs[i], i));

    int lvls = irand(3, 5);
    int pisoheight = 7;
    float hexheight = 0.8f;
    for(int y = 0; y < lvls; y++)
    {
        vector<int> v;
        if(y == 0)
        {
            v.push_back(-10);
            v.push_back(10);
        }
        else
        {
            int ct = irand(1, 3);
            for(int i = 0; i < ct*2; i++)
                v.push_back(irand(-8+y, 8-y));
        }

        sort(v.begin(), v.end());

        for(int i = 0; i < v.size()/2; i++)
            for(int x = v[i*2]; x <= v[i*2+1]; x++)
            {
                int h = 1;
                if(irand(0, 10) == 0)
                    h = irand(3,  pisoheight);

                int type = irand(1, 10);
                for(int y2 = 0; y2 < h; y2++)
                {
                    float yy = 0.4f + (y*pisoheight+y2)*hexheight;
                    switch(type)
                    {
                    case 0:
                    case 1:
                        actors.push_back(new TrapHexagon(this, vec3(x*2, yy, 1)));
                        break;
                    case 2:
                    case 3:
                        actors.push_back(new BouncyHexagon(this, vec3(x*2, yy, 0)));
                        break;
                    default:
                        actors.push_back(new StaticHexagon(this, vec3(x*2, yy, 0)));
                        break;
                    }
                }
            }
    }

/*
    for (int k = -3; k < 0; k++) assetList.push_back(Asset(HEXTRAP, vec3(k*3, 8, 0)));
    for (int k = 0; k < 3; k++) assetList.push_back(Asset(HEXBOUNCY, vec3(k*2, k*2+5, 0)));
    for (int k = 3; k < 6; k++) assetList.push_back(Asset(HEXEXPLOSIVE, vec3(k*2, 10, 0)));
    for (int k = 6; k < 9; k++) assetList.push_back(Asset(HEXSOLID, vec3(k*2, 10, 0)));
    for (int k = 2; k < 6; k++) assetList.push_back(Asset(HEXFLOATING, vec3(-16+0.5*k, k, 0)));
*/

    deadTimer = 6;
    spawnTimer = 1;
    tntTimer = 10;

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
        nextScene = new GameScene(FREEMODE, numPlayers);

    if(spawnTimer < 0)
    {
        spawnTimer = 16;
        actors.push_back(new ShooterEnemy(this, vec3(frand(10), 40, 0)));
    }
    else
        spawnTimer -= dt;

    if(tntTimer < 0)
    {
        tntTimer = 25;
        actors.push_back(new ExplosiveHexagon(this, vec3(frand(10), 40, 0)));
    }
    else
        tntTimer -= dt;

    spawnTimer <= dt;
    if (!playerList.empty())
    {
        vec3 minPos, maxPos;
        minPos = maxPos = playerList[0]->p;
        for (int i = 1; i < playerList.size(); i++)
        {
            PlayerActor* pl = playerList[i];
            if (pl->p.x < minPos.x) minPos.x = pl->p.x;
            if (pl->p.x > maxPos.x) maxPos.x = pl->p.x;
            if (pl->p.y < minPos.y) minPos.y = pl->p.y;
            if (pl->p.y > maxPos.y) maxPos.y = pl->p.y;
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

            b2Fixture* fixt;
            float rayFract = GetRayCastDistance(b2Vec2(pos.x, pos.y), b2Vec2((*it)->p.x, (*it)->p.y), fixt);
            if (rayFract >= 1 - 0.5/dist - 0.1 && (dist < bestDist || bestDist == -1))
            {
                best = dynamic_cast<PlayerActor*>(*it);
                bestDist = dist;
            }
        }
    }
    return best;
}
