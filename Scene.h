#ifndef _SCENE_H
#define _SCENE_H

#include "Particle.h"
#include<list>
#include "util.h"
#include <Box2D/Box2D.h>

using namespace std;

class Actor;

class Scene : public b2ContactListener
{
public:
    vec3 cameraPos;
    vec3 cameraLookAt;

	list<Particle> particles;
	list<Actor*> actors;
    b2World world;
	Texture particleTex;

    Scene* nextScene;
    float particlePosMult;

	Scene();
	~Scene();
	virtual void update();
	virtual void render();
	void renderParticles();
    void renderParticlePass();

	void addParticle(const Particle& p);
};

#endif
