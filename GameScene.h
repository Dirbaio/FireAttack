#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "Model.h"

class PlayerActor;

enum GameMode
{
    FREEMODE,
    VERSUS
};

class GameScene : public Scene
{
    public:

    GameScene(GameMode mode, int numPlayers);

    float deadTimer;
    float spawnTimer;
    float tntTimer;

    int winner;
    int endState;

    int numPlayers;
    GameMode gameMode;

    virtual void update();
    virtual void render();
    virtual void renderHud();
    vector<float> vertexs, texcoords, colors;
    vector<PlayerActor*> getPlayerList();
    PlayerActor* getPlayer();

    PlayerActor* GetNearestPlayerInView(vec3 pos);
};

#endif // GAMESCENE_H
