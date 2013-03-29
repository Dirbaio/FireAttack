#ifndef MODELACTOR_H
#define MODELACTOR_H

#include "Actor.h"
#include "Model.h"

class GameScene;

class ModelActor : public Actor
{

private:
    Model* mdl;

public:
    ModelActor(GameScene* sc, string mdlName);

    virtual void update();
    virtual void render();
};

#endif // MODELACTOR_H
