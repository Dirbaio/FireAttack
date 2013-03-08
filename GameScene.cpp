#include "GameScene.h"
#include "BoxActor.h"
#include "GroundActor.h"
#include "PlayerActor.h"
#include "FireActor.h"
#include "WallActor.h"

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

    for (int i = 0; i < v.size(); i++)
    {
        if (vis[i]) continue;
        vis[i] = true;
        list<line> l;
        l.push_back(v[i]);

        for (int j = i+1; j < v.size(); j++)
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

GameScene::GameScene() : mdl("test.obj")
{
    for(int i = 0; i < 10; i++)
        actors.push_back(new BoxActor(this, false));
    actors.push_back(new PlayerActor(this));
//    actors.push_back(new WallActor(this, -10, -1));
//    actors.push_back(new WallActor(this, 10, 1));


    /*
    vector<b2Vec2> points2;
    points2.push_back(b2Vec2(-10, 100));
    points2.push_back(b2Vec2(-10, 0));
    points2.push_back(b2Vec2(10, 0));
    points2.push_back(b2Vec2(10, 100));

    b2BodyDef groundBodyDef;
    b2Body* groundBody;

    groundBodyDef.position.Set(0, 0);
    groundBody = world.CreateBody(&groundBodyDef);
    b2ChainShape chain;
    chain.CreateChain(&points2[0], points2.size());
    groundBody->CreateFixture(&chain, 0.0f);
*/

    vector<line> lines;
    for(int i = 0; i < int(mdl.vertexArray.size()); i += 3)
    {
        vec3 a = mdl.vertexArray[i+0];
        vec3 b = mdl.vertexArray[i+1];
        vec3 c = mdl.vertexArray[i+2];

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

    cerr << polygons.size() << endl;

    for (int i = 0; i < int(polygons.size()); i++)
    {
        b2BodyDef bodyDef;
        b2Body* body;
        int n = polygons[i].size() + 1;

        bodyDef.position.Set(0, 0);
        body = world.CreateBody(&bodyDef);

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

    deadTimer = 6;
    spawnTimer = 1;

    song1 = loadSound("game1.wav");
    song2 = loadSound("game2.wav");
    song.setBuffer(*song1);
    song.setLoop(true);
    song.play();
    awesome = false;
}

void GameScene::goAwesome()
{
    if(awesome) return;
    awesome = true;
    song.setBuffer(*song2);
    song.setLoop(true);
    song.play();
}

PlayerActor* GameScene::getPlayer()
{
    for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
    {
        PlayerActor* r = dynamic_cast<PlayerActor*>(*it);
        if(r) return r;
    }

    return NULL;
}

void GameScene::update()
{
    Scene::update();
    if(awesome)
    {
        float pos = song.getPlayingOffset().asSeconds();
        pos *= 132.0f / 60.0f;
        pos = fmod(pos, 1)*3 - 2;
        particlePosMult = 1 + pos;
    }

    PlayerActor* pl = getPlayer();
    if(!pl)
    {
        if(deadTimer == 6)
        {
            song.stop();
            song.setBuffer(*song2);
            song.setPlayingOffset(seconds(60+30.9df));
            song.play();
        }
        deadTimer -= dt;
    }
    if(deadTimer <= 0 && !nextScene)
        nextScene = new GameScene();

    spawnTimer -= dt;
    if(spawnTimer < 0)
    {
        spawnTimer = 1 + frand(1);
//        if(actors.size() < 20)
            actors.push_back(new BoxActor(this, prand(awesome?0.9:0.1)));

        if(awesome && prand(0.05))
            for(int i = 0; i < 3; i++)
                actors.push_back(new BoxActor(this, true));
    }
}

void GameScene::render()
{
    Scene::render();
/*    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // [!] Juanking SFML

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //Set the vertex and color pointers
    glVertexPointer(3, GL_FLOAT, 0, &vertexs[0]);
    glColorPointer(3, GL_FLOAT, 0, &colors[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);

    //Draw the cube using the previously specified vertices & colors, and the specified cube indices
    glDrawArrays(GL_QUADS, 0, vertexs.size()/3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
*/
    mdl.draw();
}

void GameScene::BeginContact(b2Contact *contact)
{
    Actor* a = (Actor*) contact->GetFixtureA()->GetBody()->GetUserData();
    Actor* b = (Actor*) contact->GetFixtureB()->GetBody()->GetUserData();

    if(a && b)
    {
        if(!a->collided(b))
            b->collided(a);
    }
    else if(a)
        a->collidedWithGround();
    else if(b)
        b->collidedWithGround();
}
