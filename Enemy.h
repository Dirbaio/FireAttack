#ifndef ENEMY_H
#define ENEMY_H

#include "PlayerActor.h"
#include "Actor.h"
#include "GameScene.h"

class Enemy : public Actor
{
private:
    PlayerActor* player;
    GameScene* sc; //esto esta to sidoso, Actor ya tiene un GameScene* sc, si heredas de Actor no declares otra variable
                   //con el mismo nombre. Simplemente llama a Actor(sc) en el constructor de Enemy(no en el body del
                   //constructor, si no en la  initialisation list), y ya tendrás sc como puntero a la escena padre,
                   //ya que Actor(sc) ya te asigna la scene que le pases a GameScene* sc :)
public:
    Enemy(GameScene* sc);
    virtual void update();
    virtual void render();
};

#endif // ENEMY_H
