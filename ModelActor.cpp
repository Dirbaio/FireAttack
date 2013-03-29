#include "ModelActor.h"
#include "GameScene.h"
#include "util.h"

#include <vector>
#include <list>

struct line
{
    vec2 a, b;
    line(float x1, float y1, float x2, float y2)
    {
        a = vec2(x1,y1);
        b = vec2(x2,y2);
    }
};

bool intersection(vec3 a, vec3 b, vec3& out)
{
    if(a.z < 0 && b.z < 0) return false;
    if(a.z > 0 && b.z > 0) return false;
    if(a.z == 0 && b.z == 0) return false;

    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dz = b.z - a.z;

    out.z = 0;
    out.x = a.x - dx/dz*a.z;
    out.y = a.y - dy/dz*a.z;

    return true;
}

bool pointCollision(const line& a, line& b)
{
    float eps = 0.01;
    if (a.b.x-eps <= b.a.x && a.b.x+eps >= b.a.x && a.b.y-eps <= b.a.y && a.b.y+eps >= b.a.y)
        return true;

    if (a.a.x-eps <= b.b.x && a.a.x+eps >= b.b.x && a.a.y-eps <= b.b.y && a.a.y+eps >= b.b.y)
        return true;

    if (a.b.x-eps <= b.b.x && a.b.x+eps >= b.b.x && a.b.y-eps <= b.b.y && a.b.y+eps >= b.b.y)
    {
        vec2 aux = b.a;
        b.a = b.b;
        b.b = aux;
        return true;
    }

    if (a.a.x-eps <= b.a.x && a.a.x+eps >= b.a.x && a.a.y-eps <= b.a.y && a.a.y+eps >= b.a.y)
    {
        vec2 aux = b.a;
        b.a = b.b;
        b.b = aux;
        return true;
    }
    return false;
}

vector<list<line> > connectLines(const vector<line>& v)
{
    vector<list<line> > res;
    vector<bool> vis (v.size(), false);

    for (int i = 0; i < int(v.size()); i++)
    {
        if (vis[i]) continue;
        vis[i] = true;
        list<line> l;
        l.push_back(v[i]);

        for (int j = i+1; j < int(v.size()); j++)
        {
            if (vis[j]) continue;
            line x = v[j], a = l.front(), b = l.back();

            if (pointCollision(a, x))
            {
                l.push_front(x);
                vis[j] = true;
                j = i+1;
            }
            else if (pointCollision(b, x))
            {
                l.push_back(x);
                vis[j] = true;
                j = i+1;
            }

        }
        res.push_back(l);
    }
    return res;
}


ModelActor::ModelActor(GameScene* sc, string mdlName) : Actor(sc)
{
    mdl = new Model(mdlName);
    vector<line> lines;
    for(int i = 0; i < int(mdl->vertexArray.size()); i += 3)
    {
        vec3 a = mdl->vertexArray[i+0];
        vec3 b = mdl->vertexArray[i+1];
        vec3 c = mdl->vertexArray[i+2];

        vector<vec3> pts;
        vec3 p;

        if(intersection(a, b, p)) pts.push_back(p);
        if(intersection(c, b, p)) pts.push_back(p);
        if(intersection(c, a, p)) pts.push_back(p);

        if(pts.size() == 2)
        {
            lines.push_back(line(pts[0].x, pts[0].y,pts[1].x, pts[1].y));
        }
    }

    vector<list<line> > polygons = connectLines(lines);

    for (int i = 0; i < int(polygons.size()); i++)
    {
        b2BodyDef bodyDef;
        b2Body* body;
        int n = polygons[i].size() + 1;

        bodyDef.position.Set(0, 0);
        body = sc->world.CreateBody(&bodyDef);

        b2Vec2 vert[n];
        list<line>::iterator it = polygons[i].begin();
        vert[0].Set(it->a.x, it->a.y);

        for (int j = 1; j < n; j++)
        {
            vert[j].Set(it->b.x, it->b.y);
            it++;
        }

        b2ChainShape chain;
        chain.CreateChain(vert, n);
        body->CreateFixture(&chain, 0.0f);
    }
}

void ModelActor::update()
{

}

void ModelActor::render()
{
    mdl->draw();
}
