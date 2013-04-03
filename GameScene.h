#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "Model.h"

class PlayerActor;

class GameScene : public Scene
{
    public:

    GameScene();

    SoundBuffer* song1;
    SoundBuffer* song2;
    Sound song;

    bool awesome;
    void goAwesome();

    float deadTimer;
    float spawnTimer;

    int numPlayers;

    virtual void update();
    virtual void render();
    vector<float> vertexs, texcoords, colors;
    vector<PlayerActor*> getPlayerList();
    PlayerActor* getPlayer();

    PlayerActor* GetNearestPlayerInView(vec3 pos);
};

#endif // GAMESCENE_H
