#ifndef _FIREACTOR_H
#define _FIREACTOR_H
#include "Actor.h"

class GameScene;

struct BulletConfig
{
    vec3 col1, col2, col3, col4;
};

class FireActor : public Actor
{
	public:
	
	float size;
	float timeSinceGround;
    GameScene* gsc;
    FireActor(vec3 pos, vec3 vel, GameScene* sc, BulletConfig* config);
    bool green;

	Texture* playerTex;

    int bounceCount;
	
	virtual void render();
    virtual void update();
    virtual void collided(Actor *b);
    virtual bool collidedWithGround();
};

#endif
