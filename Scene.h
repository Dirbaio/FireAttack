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
    Shader* defaultShader;

    int lightCount; //For debug
    float inverseLookAt[16];

	Scene();
	~Scene();
	virtual void update();
	virtual void render();
    void renderSingle();
    void renderParticles(bool reflex);
    void renderLights();

	void addParticle(const Particle& p);
    void BeginContact(b2Contact *contact);

    float GetRayCastDistance(b2Vec2 p1, b2Vec2 p2);

    void makeExplosion(vec3 pos, float force, bool explodes, bool destroys);
};

#endif
