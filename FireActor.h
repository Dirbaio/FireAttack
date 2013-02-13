#ifndef _FIREACTOR_H
#define _FIREACTOR_H
#include "Actor.h"

class GameScene;

class FireActor : public Actor
{
	public:
	
	float size;
	float timeSinceGround;
    GameScene* gsc;
    FireActor(vec3 pos, vec3 vel, GameScene* sc, bool green);
    bool green;

	Texture* playerTex;
	
	float rotx, roty;
    int bounceCount;
	
	virtual void render();
	virtual void update();
    virtual bool collided(Actor *b);
    virtual bool collidedWithGround();
};

#endif
