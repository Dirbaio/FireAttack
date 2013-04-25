#include "Scene.h"
#include "Actor.h"
#include "Hexagon.h"
#include "ExplosionActor.h"

class MyRayCastCallback : public b2RayCastCallback
{
public:

    b2Fixture* m_fixture;
    b2Vec2 m_point;
    b2Vec2 m_normal;
    float32 m_fraction;

    MyRayCastCallback()
    {
        m_fixture = NULL;
    }

    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
    {
        m_fixture = fixture;
        m_point = point;
        m_normal = normal;
        m_fraction = fraction;
        return fraction;
    }
};

Scene::Scene() : world(b2Vec2(0.0f, -10.0f))
{
	cameraLookAt = vec3(0, 0, 0);
    cameraPos = vec3(0, 20, 20);
    world.SetContactListener(this);

    nextScene = NULL;
    particlePosMult = 1;
    defaultShader = loadShader("vertex.glsl", "fragment-model.glsl");
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
    int velIters = 60;
    int posIters = 30;
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

    bool deleting = true;
    while(deleting)
    {
        deleting = false;
        for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); )
        {
            if(!(*it)->alive)
            {
                list<Actor*>::iterator it2 = it;
                it2++;
                if((*it)->body)
                    world.DestroyBody((*it)->body);
                delete *it;
                actors.erase(it);
                it = it2;
                deleting = true;
            }
            else
                it++;
        }
    }
	
    for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
	{
        (*it)->update();
        (*it)->updateEmitters();
    }

}

void Scene::renderSingle(bool isReflection)
{

    for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
    {
        Actor* act = *it;
        Shader* sh = act->shader == NULL ? defaultShader : act->shader;
        Shader::bind(sh);

        int program;
        glGetIntegerv(GL_CURRENT_PROGRAM, &program); //Trololo
        GLint loc = glGetUniformLocation(program, "time");
        if(loc != -1)
            glUniform1f(loc, tim);
        loc = glGetUniformLocation(program, "inverseLookAt");
        if(loc != -1)
            glUniformMatrix4fv(loc, 1, false, inverseLookAt);
        loc = glGetUniformLocation(program, "isReflection");
        if(loc != -1)
            glUniform1i(loc, isReflection);
        act->render();
        Shader::bind(NULL);
    }

}

void Scene::renderHud() {}
void Scene::render()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(  cameraPos.x, cameraPos.y, cameraPos.z,
				cameraLookAt.x, cameraLookAt.y, cameraLookAt.z,
                0, 1, 0);

    glDisable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    vec3 camVec = cameraLookAt - cameraPos;
    normalize(camVec);
    setCameraVec(camVec, false);

    float modelview[16];

    //Normal rendering
    glPolygonOffset(1,1);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    gluInvertMatrix(modelview, inverseLookAt);
    glCullFace(GL_BACK);
    renderSingle(false);

    //Reflections
    glPolygonOffset(0,0);
    glCullFace(GL_FRONT);
    glPushMatrix();
    glScalef(1, -1, 1);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    gluInvertMatrix(modelview, inverseLookAt);
    renderSingle(true);
    glPopMatrix();

}

vector<vec3> vtxArray;
vector<float> texArray;
vector<color> colArray;

void Scene::renderParticles(bool reflex)
{
    vtxArray.resize(0);
    texArray.resize(0);
    colArray.resize(0);

    vtxArray.reserve(particles.size()*4);
    texArray.reserve(particles.size()*4*4);
    colArray.reserve(particles.size()*4);

    Particle p;
    p.life = 1;
    p.startingLife = 1;
    for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
        if((*it)->renderParticle(p))
            p.render(vtxArray, texArray, colArray, reflex);
    for(list<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
        it->render(vtxArray, texArray, colArray, reflex);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, &vtxArray[0]);
    glTexCoordPointer(4, GL_FLOAT, 0, &texArray[0]);
    glColorPointer(4, GL_FLOAT, 0, &colArray[0]);

    glDrawArrays(GL_QUADS, 0, vtxArray.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void Scene::renderLights()
{
    vtxArray.resize(0);
    texArray.resize(0);
    colArray.resize(0);

    Particle p;
    p.life = 1;
    p.startingLife = 1;
    for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
        if((*it)->renderParticle(p))
            p.renderLight(vtxArray, texArray, colArray);

    for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
    {
        Actor* a = *it;
        for(list<ParticleEmitter>::iterator it2 = a->emitters.begin(); it2 != a->emitters.end(); it2++)
            it2->renderLight(vtxArray, texArray, colArray);
    }

    for(list<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
        if(it->isLight)
            it->renderLight(vtxArray, texArray, colArray);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, &vtxArray[0]);
    glTexCoordPointer(4, GL_FLOAT, 0, &texArray[0]);
    glColorPointer(4, GL_FLOAT, 0, &colArray[0]);

    lightCount = vtxArray.size() / 4;
    glDrawArrays(GL_QUADS, 0, vtxArray.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void Scene::addParticle(const Particle& p) 
{
	particles.push_back(p);
}

void Scene::BeginContact(b2Contact *contact)
{
    Actor* a = (Actor*) contact->GetFixtureA()->GetBody()->GetUserData();
    Actor* b = (Actor*) contact->GetFixtureB()->GetBody()->GetUserData();

    if(a && b)
    {
        a->collided(b);
        b->collided(a);
    }
    else if(a)
        a->collidedWithGround();
    else if(b)
        b->collidedWithGround();
}

float Scene::GetRayCastDistance(b2Vec2 p1, b2Vec2 p2, b2Fixture*& fixt)
{
    MyRayCastCallback callback;
    b2Vec2 point1(p1.x, p1.y);
    b2Vec2 point2(p2.x, p2.y);
    world.RayCast(&callback, point1, point2);
    if (callback.m_fixture == NULL) return 123456;
    fixt = callback.m_fixture;
    return callback.m_fraction;
}

void Scene::makeExplosion(vec3 pos, float force, bool explodes, bool destroys, float inc)
{
    actors.push_back(new ExplosionActor(this, pos, force, explodes, destroys, inc));
}
