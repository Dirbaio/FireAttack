#include "Hexagon.h"
#include "util.h"
#include "GameScene.h"
#include "FireActor.h"
#include "PlayerActor.h"
#include "Enemy.h"

Hexagon::Hexagon(GameScene* sc, vec3 pos, bool movable, bool rotable, bool destructible, float reg, int l) : Actor(sc)
{
    rot = 0;
    sx = sz = 1.0;
    sy = 0.4;
    p.x = pos.x;
    p.y = pos.y;
    p.z = pos.z;

    regen_time = 0;
    regen_time_window = reg;
    life = max_life = l;

    timer = timer2 = timer3 = colTimer = 0.0;

    dying = dead = false;
    dying_time = 0.0;

    if (movable)
        dying_time_window = 1.0;
    else
        dying_time_window = 0.0;

    this->movable = movable;
    this->rotable = rotable;
    this->destructible = destructible;

    b2BodyDef bodyDef;
    if (movable) bodyDef.type = b2_dynamicBody;
    else bodyDef.type = b2_staticBody;
    bodyDef.position.Set(p.x, p.y);
    body = sc->world.CreateBody(&bodyDef);
    body->SetLinearDamping(1.0f);
    b2PolygonShape box;
    box.SetAsBox(1.0, 0.4);
    b2FixtureDef fixture;
    fixture.density = 1.0f;
    fixture.friction = 0.0f;
    fixture.shape = &box;
    fixture.restitution = 0.3f;
    fixture.userData = this;
    body->SetFixedRotation(!rotable);
    body->CreateFixture(&fixture);
    body->SetUserData(this);

    shader = loadShader("vertex.glsl", "fragment-hexagons.glsl");
}

void Hexagon::update()
{
    timer += dt;
    timer2 += dt*2;
    timer3 += dt*3;
    colTimer += dt;

    timer2 = min(timer2, 1.0f);
    timer3 = min(timer3, 1.0f);

    if (life == max_life)
        regen_time = 0;
    else
    {
        regen_time += dt;
        if (regen_time >= regen_time_window)
        {
            regen_time = 0;
            life++;
        }
    }
    if (dying && !dead)
    {
        dying_time += dt;
        if (dying_time >= dying_time_window)
            dead = true;
        die();
    }
}

void Hexagon::render()
{
    shader->setParameter("time", timer);
    shader->setParameter("time2", timer2);
    shader->setParameter("time3", timer3);

    glPushMatrix();

    glTranslatef(p.x, p.y, p.z);
    glRotatef(toDeg(body->GetAngle()), 0, 0, 1);
    glRotatef(toDeg(rot), 0, 1, 0);
    glScalef(sx, sy, sz);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //Set the vertex and color pointers
    glVertexPointer(3, GL_FLOAT, 0, &hexVert[0]);
    glNormalPointer(GL_FLOAT, 0, &hexNorm[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &hexTexCoord[0]);

    //Draw the cube using the previously specified vertices & colors, and the specified cube indices
    glDrawArrays(GL_TRIANGLES, 0, 72);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
}

void Hexagon::collided(Actor* b)
{
    if (dynamic_cast<FireActor*>(b))
    {
        if (destructible && !dying && !dead)
        {
            life--;
            regen_time = 0;
            if (life == 0)
                die();
        }
    }
    if (colTimer >= 0.4 && (dynamic_cast<PlayerActor*>(b) || dynamic_cast<FireActor*>(b) || dynamic_cast<Enemy*>(b)))
        timer2 = timer3 = colTimer = 0.0;
}

void Hexagon::die()
{
    if (dead)
    {
        explode();
    }
    if (!dying)
    {
        body->SetFixedRotation(false);
        body->ApplyAngularImpulse(frand(0.5)*100);
        dying = true;
    }
}

void Hexagon::swapRotable()
{
    rotable = !rotable;
    body->SetFixedRotation(!rotable);
}
