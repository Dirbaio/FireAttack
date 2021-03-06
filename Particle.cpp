#include "Particle.h"

static float texcoords[4][2] = {
	{0, 0},
	{1, 0},
	{1, 1},
	{0, 1},
};

static float vtxcoords[4][3];
vec3 cameraVec;
bool reflection;
void setCameraVec(vec3 cv, bool r)
{
	cameraVec = cv;
    reflection = r;
}

Particle::Particle()
{
	life = 1;
	startingLife = 1;
	
	startSize = 0.3;
	endSize = 2;
	startCol = vec3(1, 0, 0);
	endCol = vec3(0,1, 1);
	startAlpha = 1;
	endAlpha = 0;
	
	v = vec3(frand(1.2), 4, frand(1.2));
}

void sets(float* a, vec3 b)
{
	a[0] = b.x;
	a[1] = b.y;
	a[2] = b.z;
}

void Particle::render(vector<vec3>& vtxArray, vector<float>& texArray, vector<color>& colArray, bool reflex)
{
    if(p.y < 0.0f) return;
    vec3 pp = p;
    if(reflex) pp.y = -pp.y;
    float size = (startSize * (life) + endSize*(startingLife - life)) / startingLife;
    float alpha = (startAlpha * (life) + endAlpha*(startingLife - life)) / startingLife;
    vec3 col = (startCol * (life) + endCol*(startingLife - life)) / startingLife;
    color colr;
    colr.r = col.x;
    colr.g = col.y;
    colr.b = col.z;
    colr.a = alpha;

    vec3 dx = cross(cameraVec, vec3(0, 1, 0));
    vec3 dy = cross(dx, cameraVec);
    normalize(dx);
    normalize(dy);
    float sz = size*1.5f;
    dx *= sz;
    dy *= sz;
    vtxArray.push_back(pp+dx+dy);
    vtxArray.push_back(pp+dx-dy);
    vtxArray.push_back(pp-dx-dy);
    vtxArray.push_back(pp-dx+dy);

    for(int i = 0; i < 4; i++)
    {
        texArray.push_back(pp.x);
        texArray.push_back(pp.y);
        texArray.push_back(pp.z);
        texArray.push_back(size);
    }

    colArray.push_back(colr);
    colArray.push_back(colr);
    colArray.push_back(colr);
    colArray.push_back(colr);
}

void Particle::renderLight(vector<vec3>& vtxArray, vector<float>& texArray, vector<color>& colArray)
{
    float size = (startSize * (life) + endSize*(startingLife - life)) / startingLife;
    float alpha = (startAlpha * (life) + endAlpha*(startingLife - life)) / startingLife;
    vec3 col = (startCol * (life) + endCol*(startingLife - life)) / startingLife;

    color colr;
    colr.r = col.x;
    colr.g = col.y;
    colr.b = col.z;
    colr.a = 1;

    vec3 dx = cross(cameraVec, vec3(0, 1, 0));
    vec3 dy = cross(dx, cameraVec);
    normalize(dx);
    normalize(dy);
    float sz = size * alpha * 40.0f;
    dx *= sz;
    dy *= sz;
    vtxArray.push_back(p+dx+dy);
    vtxArray.push_back(p+dx-dy);
    vtxArray.push_back(p-dx-dy);
    vtxArray.push_back(p-dx+dy);

    for(int i = 0; i < 4; i++)
    {
        texArray.push_back(p.x);
        texArray.push_back(p.y);
        texArray.push_back(p.z);
        texArray.push_back(size*alpha);
    }

    colArray.push_back(colr);
    colArray.push_back(colr);
    colArray.push_back(colr);
    colArray.push_back(colr);
}

void Particle::update()
{
	v += a*dt;
	p += v*dt;
	life -= dt;
}

float Particle::getZOrder() const 
{
	return dot(p, cameraVec);
}
