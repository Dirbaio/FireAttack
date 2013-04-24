#include "PlayerActor.h"
#include "GameScene.h"
#include "FireActor.h"
#include "BoxActor.h"

PlayerActor::PlayerActor(GameScene* sc) : Actor(sc)
{
    this->gsc = sc;

    p.x = 0;
    p.y = 0;
    size = 0.5;

    b2BodyDef bodyDef;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.type = b2_dynamicBody;
    body = sc->world.CreateBody(&bodyDef);
    body->SetLinearDamping(1.0f);
    b2CircleShape box;
    box.m_radius = size/2;
    b2FixtureDef fixture;
    fixture.density = 1.0f;
    fixture.friction = 0.0f;
    fixture.shape = &box;
    fixture.restitution = 0.3f;
    fixture.userData = this;
    body->CreateFixture(&fixture);

    playerTex = loadTexture("player.jpg");


    ParticleEmitter pe (this);
    pe.period = 1/500.0;
    pe.startAlpha = 0.1;
    pe.randPos = RandomVec(size/4);
    pe.startSize = size/3;
    pe.endSize = size/16;
    pe.life = 0.01;
    pe.startCol = vec3(1, 1, 0.5);
    pe.endCol = vec3(1, 1, 1);
    pe.actorVelMult = 1;
    emitters.push_back(pe);

    pe.period = 1/1000.0;
    pe.randPos = RandomVec(size/2);
    pe.startSize = size/3;
    pe.endSize = size/16;
    pe.life = 1;
    pe.startAlpha = 0.2;
    pe.startCol = vec3(1, 0, 0);
    pe.endCol = vec3(1, 1, 0);
    pe.actorVelMult = 0.9;
    emitters.push_back(pe);

    pe.period = 1/1000.0;
    pe.startAlpha = 0.2;
    pe.randPos = RandomVec(size/3);
    pe.randVel = RandomVec(size/3);
    pe.startSize = size/2;
    pe.endSize = size/10;
    pe.life = 1;
    pe.startCol = vec3(1, 0, 0);
    pe.endCol = vec3(0, 1, 0.4);
    pe.actorVelMult = 0.9;
    sizeEmitter = addEmitter(pe);

    body->SetUserData(this);
    wasMouseDown = false;
    mouseDownTime = 0;
    shootCooldown = 0;
}

void PlayerActor::update()
{
    sizeEmitter->randVel.rad = min(3.0f, norm(v)*0.3f);

    float f = 5;
    if((Keyboard::isKeyPressed(Keyboard::Space) || Joystick::getAxisPosition(0, Joystick::Y) < -50) && p.y < 0.3)
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, 7.0f));
    if(Keyboard::isKeyPressed(Keyboard::A))
        body->ApplyForceToCenter(b2Vec2(-f, 0));
    if(Keyboard::isKeyPressed(Keyboard::D))
        body->ApplyForceToCenter(b2Vec2(f, 0));
    if(Keyboard::isKeyPressed(Keyboard::S))
        body->ApplyForceToCenter(b2Vec2(0, -f));
    float val = Joystick::getAxisPosition(0, Joystick::X);
    body->ApplyForceToCenter(b2Vec2(f*val/100, 0));

    if(p.x < -10)
        body->SetLinearVelocity(b2Vec2(10.0f, body->GetLinearVelocity().y));
    if(p.x > 10)
        body->SetLinearVelocity(b2Vec2(-10.0f, body->GetLinearVelocity().y));

    sc->cameraLookAt = vec3(p.x/2, 3, 0);
    float weight = exp(-dt*6);
    sc->cameraPos = sc->cameraPos * weight + (vec3(p.x/2, p.y , 0) + vec3(0, 2, 7)) * (1-weight);
/*
    if(Mouse::isButtonPressed(Mouse::Left))
        mouseDownTime += dt;
    else
        mouseDownTime = 0;

    particlePosMult = 1+mouseDownTime;
*/

    bool shoot = false;
    vec3 dir;

    if(Mouse::isButtonPressed(Mouse::Left))
    {
        Vector2i ppos = Mouse::getPosition(*theApp);

        vec2 pos (float(ppos.x) - theApp->getSize().x / 2,
                  theApp->getSize().y / 2 - float(ppos.y));
        pos /= float(theApp->getSize().y/2);
        //Awful math below :S

        vec3 camVec = sc->cameraLookAt - sc->cameraPos;
        normalize(camVec);
        vec3 up2 (0, 1, 0);
        vec3 right = cross(camVec, up2);
        normalize(right);
        vec3 up = cross(right, camVec);
        normalize(up);

        vec3 camVec2 = camVec + right*pos.x + up*pos.y;
        vec3 res = sc->cameraPos;
        camVec2 *= res.z/camVec2.z;
        res -= camVec2;

        dir = res - p;
        shoot = true;
    }

    vec3 joy (0, 0, 0);
    joy.x = sf::Joystick::getAxisPosition(0, Joystick::Z) / 100.0f;
    joy.y = -sf::Joystick::getAxisPosition(0, Joystick::R) / 100.0f;
    if(joy.x*joy.x + joy.y*joy.y > 0.5)
    {
        dir = joy;
        shoot = true;
    }

    if(shootCooldown > 0)
        shootCooldown -= dt;

    if(shoot && shootCooldown <= 0)
    {
        normalize(dir);

        FireActor* bullet = new FireActor(p+dir*0.4f, dir*10.0f, gsc, false);
        sc->actors.push_back(bullet);
        float knockback = 0.5;
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x-dir.x*knockback, body->GetLinearVelocity().y-dir.y*knockback));

        shootCooldown = 0.05;
    }

    wasMouseDown = Mouse::isButtonPressed(Mouse::Left);
}

void PlayerActor::explode()
{
    ParticleEmitter e(this);
    e.randVel = RandomVec(3);
    e.life = 1;
    e.startAlpha = 1;
    e.endAlpha = 0;
    e.startCol = vec3(1, 0, 0);
    e.endCol = vec3(1, 0.4, 0);
    e.startSize = 0;
    e.endSize = 1;
    e.boom(2000);
    e.randVel = RandomVec(2.5);
    e.life = 1;
    e.startAlpha = 1;
    e.endAlpha = 0;
    e.startCol = vec3(1, 0, 0);
    e.endCol = vec3(1, 0, 1);
    e.startSize = 0;
    e.endSize = 1;
    e.boom(2000);

    e.randVel = RandomVec(0, 5, 0);
    e.randPos = RandomVec(0, 1, 0);
    e.life = 1;
    e.startAlpha = 1;
    e.endAlpha = 0;
    e.startCol = vec3(1, 1, 1);
    e.endCol = vec3(1, 0.6d, 0);
    e.startSize = 0;
    e.endSize = 1;
    e.boom(2000);

    alive = false;
}

void PlayerActor::render()
{
//    glColor3f(1, 1, 1);
//    playerTex->bind();
//    renderCube(p.x, p.y, p.z, size/2, size/2, size/2, body->GetAngle());
}

bool PlayerActor::collided(Actor *b)
{
    if(dynamic_cast<BoxActor*>(b))
    {
        explode();
        return true;
    }
    return false;
}

bool PlayerActor::collidedWithGround()
{
    return false;
}
