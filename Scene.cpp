#include "Scene.h"
#include "Actor.h"

Scene::Scene() : world(b2Vec2(0.0f, -10.0f))
{
	cameraLookAt = vec3(0, 0, 0);
    cameraPos = vec3(0, 20, 20);
	particleTex.loadFromFile("particle.png");
    world.SetContactListener(this);

    nextScene = NULL;
    particlePosMult = 1;
}

Scene::~Scene()
{
	for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); it++)
	{
		Actor* a = *it;
		delete a;
	}
}

void Scene::update()
{
    int velIters = 5;
    int posIters = 2;
    world.Step(dt, velIters, posIters);
/*
    float step = 1.0f / 60.0f;
    float left = dt;
//    if(left > 0.05) left = 0.05;
    while(left > step)
    {
        world.Step(step, velIters, posIters);
        left -= step;
    }

    if(left > 0)
        world.Step(left, velIters, posIters);
    */
	for(list<Particle>::iterator it = particles.begin(); it  != particles.end(); )
	{
		it->update();
		if(it->life <= 0)
		{
			list<Particle>::iterator it2 = it;
			it2++;
			particles.erase(it);
			it = it2;
		}
		else
			it++;
	}
	
	for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); )
	{
        (*it)->updateEmitters();
        (*it)->update();
		if(!(*it)->alive)
		{
			list<Actor*>::iterator it2 = it;
			it2++;
            if((*it)->body)
                world.DestroyBody((*it)->body);
			actors.erase(it);
			it = it2;
		}
		else
			it++;
	}
}

void Scene::render()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(  cameraPos.x, cameraPos.y, cameraPos.z, 
				cameraLookAt.x, cameraLookAt.y, cameraLookAt.z,
                0, 1, 0);

    glDisable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);

    for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
		(*it)->render();

    vec3 camVec = cameraLookAt - cameraPos;
    normalize(camVec);
    setCameraVec(camVec, false);
}

vector<vec3> vtxArray;
vector<vec2> texArray;
vector<color> colArray;

void Scene::renderParticles()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    vec3 camVec = cameraLookAt - cameraPos;
    normalize(camVec);
    setCameraVec(camVec, false);
    renderParticlePass();

    glDepthFunc(GL_GREATER);

    camVec.y = -camVec.y;
    setCameraVec(camVec, true);

    glPushMatrix();
    glScalef(1, -1, 1);
    renderParticlePass();
    glPopMatrix();

    glDepthFunc(GL_LESS);
}

void Scene::renderParticlePass()
{
    if(particles.size() == 0)
        return;


    //SORT DA PARTICLES!!!
//	particles.sort();
	
    //Render them!
    Texture::bind(&particleTex);

	vtxArray.resize(0);
	texArray.resize(0);
	colArray.resize(0);
	
	vtxArray.reserve(particles.size()*4);
	texArray.reserve(particles.size()*4);
	colArray.reserve(particles.size()*4);
	
	for(list<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
		it->renderArray(vtxArray, texArray, colArray);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vtxArray[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texArray[0]);
	glColorPointer(4, GL_FLOAT, 0, &colArray[0]);
	
/*	cout<<vtxArray.size()<<endl;
	cout<<texArray.size()<<endl;*/

    glDrawArrays(GL_QUADS, 0, vtxArray.size());
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void Scene::addParticle(const Particle& p) 
{
	particles.push_back(p);
}

