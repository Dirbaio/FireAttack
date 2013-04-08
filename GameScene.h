#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "Model.h"
#include "Level.h"
#include "LevelIO.h"

class PlayerActor;

enum GameMode
{
    FREEMODE,
    VERSUS
};

class GameScene : public Scene
{
    public:

    GameScene(GameMode mode);

    SoundBuffer* song1;
    SoundBuffer* song2;
    Sound song;

    float deadTimer;
    float spawnTimer;

    int numPlayers;
    GameMode gameMode;

    virtual void update();
    virtual void render();
    vector<float> vertexs, texcoords, colors;
    vector<PlayerActor*> getPlayerList();
    PlayerActor* getPlayer();

    PlayerActor* GetNearestPlayerInView(vec3 pos);
};

#endif // GAMESCENE_H
