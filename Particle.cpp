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

void Particle::render()
{
	float size = (startSize * (life) + endSize*(startingLife - life)) / startingLife;
	float alpha = (startAlpha * (life) + endAlpha*(startingLife - life)) / startingLife;
	vec3 col = (startCol * (life) + endCol*(startingLife - life)) / startingLife;

	vec3 dx = cross(cameraVec, vec3(0, 1, 0));
	vec3 dy = cross(dx, cameraVec);
	normalize(dx);
	normalize(dy);
	dx *= size;
	dy *= size;
    sets(vtxcoords[0], p+dx+dy);
    sets(vtxcoords[1], p+dx-dy);
    sets(vtxcoords[2], p-dx-dy);
    sets(vtxcoords[3], p-dx+dy);
	
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glColor4f(col.x, col.y, col.z, alpha);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vtxcoords);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Particle::renderArray(vector<vec3>& vtxArray, vector<vec2>& texArray, vector<color>& colArray)
{
	float size = (startSize * (life) + endSize*(startingLife - life)) / startingLife;
	float alpha = (startAlpha * (life) + endAlpha*(startingLife - life)) / startingLife;
	vec3 col = (startCol * (life) + endCol*(startingLife - life)) / startingLife;
	color colr;
	colr.r = col.x;
	colr.g = col.y;
	colr.b = col.z;
    colr.a = alpha;
	
    if(reflection)
    {
        colr.r *= 0.7;
        colr.g *= 0.8;
        colr.b *= 1;
        colr.a *= 0.3;
    }
	vec3 dx = cross(cameraVec, vec3(0, 1, 0));
	vec3 dy = cross(dx, cameraVec);
	normalize(dx);
	normalize(dy);
	dx *= size;
	dy *= size;

	vtxArray.push_back(p+dx+dy);
	vtxArray.push_back(p+dx-dy);
	vtxArray.push_back(p-dx-dy);
	vtxArray.push_back(p-dx+dy);
	
    texArray.push_back(vec2(-1, -1));
    texArray.push_back(vec2(1, -1));
	texArray.push_back(vec2(1, 1));
    texArray.push_back(vec2(-1, 1));
	
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
