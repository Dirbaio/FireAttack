#include "PlayerActor.h"
#include "GameScene.h"
#include "FireActor.h"
#include "BouncyHexagon.h"
#include "SolidHexagon.h"
#include "MagmaHexagon.h"
#include "Enemy.h"
#include "Input.h"

PlayerActor::PlayerActor(GameScene* sc, PlayerConfig config, int numPlayer) : Actor(sc)
{
    this->gsc = sc;

    p.x = config.initPos.x;
    p.y = config.initPos.y;
    size = 0.5;

    bounce_cooldown = 0;
    bounce_cooldown_min = 0.1;
    bounce_factor_x = 1.8;
    bounce_factor_y = 3.0;

    dashCooldownTime = 0.0;
    dashCooldownTimeMax = 0.2;

    b2BodyDef bodyDef;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.type = b2_dynamicBody;
    body = sc->world.CreateBody(&bodyDef);
    body->SetLinearDamping(1.0f);
    body->SetGravityScale(2.5);
    b2CircleShape box;
    box.m_radius = size/2;
    b2FixtureDef fixture;
    fixture.density = 2.0f;
    fixture.friction = 0.0f;
    fixture.shape = &box;
    fixture.restitution = 0.3f;
    fixture.userData = this;
    body->CreateFixture(&fixture);

    ParticleEmitter pe (this);
    pe.period = 1/500.0;
    pe.startAlpha = 0.1;
    pe.randPos = RandomVec(size/4);
    pe.startSize = size/3;
    pe.endSize = size/16;
    pe.life = 0.01;
    pe.startCol = config.col1;
    pe.endCol = config.col2;
    pe.actorVelMult = 1;
    emitters.push_back(pe);

    pe.period = 1/1000.0;
    pe.randPos = RandomVec(size/2);
    pe.startSize = size/3;
    pe.endSize = size/16;
    pe.life = 1;
    pe.startAlpha = 0.2;
    pe.startCol = config.col3;
    pe.endCol = config.col4;
    pe.actorVelMult = 0.9;
    emitters.push_back(pe);

    pe.period = 1/1000.0;
    pe.startAlpha = 0.2;
    pe.randPos = RandomVec(size/3);
    pe.randVel = RandomVec(size/3);
    pe.startSize = size/2;
    pe.endSize = size/10;
    pe.life = 1;
    pe.startCol = config.col5;
    pe.endCol = config.col6;
    pe.actorVelMult = 0.9;
    sizeEmitter = addEmitter(pe);

    body->SetUserData(this);
    canDash = false;

    cfg = config;
    if (config.useWiimote) input = new Input(config.numWiimote);
    else input = new Input(config.keyMap);
}

void PlayerActor::update()
{
    input->update();

    sizeEmitter->randVel.rad = min(3.0f, norm(v)*0.3f);

    bounce_cooldown += dt;
    dashCooldownTime += dt;

    float dist = sc->GetRayCastDistance(b2Vec2(p.x, p.y), b2Vec2(p.x, p.y-size/2.0-0.04));
    bool grounded = (dist < 1.0 && dist > 0.0);
    dist = sc->GetRayCastDistance(b2Vec2(p.x-size/2.0+0.02, p.y), b2Vec2(p.x-size/2.0+0.02, p.y-size/2.0-0.04));
    grounded = grounded || (dist < 1.0 && dist > 0.0);
    dist = sc->GetRayCastDistance(b2Vec2(p.x+size/2.0-0.02, p.y), b2Vec2(p.x+size/2.0-0.02, p.y-size/2.0-0.04));
    grounded = grounded || (dist < 1.0 && dist > 0.0);

    float f = 5.0;
    if(input->getKeyPressed(JUMP) && grounded)// && p.y < 0.3)
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, 10.0f));
    if(input->getKeyPressed(JUMP) && body->GetLinearVelocity().y > 0.0f)
        body->ApplyForceToCenter(b2Vec2(0, 5.0f));
    if(input->getKeyPressed(MOVELEFT))
        body->ApplyForceToCenter(b2Vec2(-f, 0));
    if(input->getKeyPressed(MOVERIGHT))
        body->ApplyForceToCenter(b2Vec2(f, 0));

    float dashx = (input->getValue(DASHX)-128)/128.0;
    float dashy = (input->getValue(DASHY)-128)/128.0;
    float dashz = (input->getValue(DASHZ)-128)/128.0;

    if (dashCooldownTime >= dashCooldownTimeMax && canDash && (abs(dashx) > 0.7 || abs(dashz) > 0.7))
    {
        ParticleEmitter pe (this);
        pe.startAlpha = 0.1;
        pe.endAlpha = 0.6;
        pe.randPos = RandomVec(0);
        pe.randVel = RandomVec(5, CIRCLE_XZ);
        pe.startSize = size;
        pe.endSize = 0;
        pe.life = 0.2;
        pe.startCol = cfg.col1;
        pe.endCol = cfg.col2;
        pe.actorVelMult = 0;
        pe.boom(300);
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, f*3));
        canDash = false;
        dashCooldownTime = 0.0;
    }


    if(input->getKeyDown(SHOOT))
    {

        vec2 pos (input->getValue(POINTERX) - theApp->getSize().x / 2,
                  theApp->getSize().y / 2 - input->getValue(POINTERY));
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

        vec3 dir = res - p;
        normalize(dir);

        if (gsc->GetRayCastDistance(b2Vec2(p.x,p.y), b2Vec2(p.x+dir.x*size, p.y+dir.y*size)) > 0.8)
        {

            BulletConfig config;
            config.col1 = cfg.col1;
            config.col2 = cfg.col2;
            config.col3 = cfg.col3;
            config.col4 = cfg.col4;
            FireActor* bullet = new FireActor(p+dir*0.4f, dir*10.0f, gsc, &config);
            sc->actors.push_back(bullet);
            //body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x-dir.x, body->GetLinearVelocity().y-dir.y));
        }
    }

    if (p.y <= -0.2)
        explodeWater();
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

    sc->makeExplosion(p, 45.0, false, true, 10.0);

    alive = false;
}

void PlayerActor::explodeWater()
{
    ParticleEmitter e(this);
    e.a = vec3(0, -7, 0);
    e.lightPermil = 0;
    e.randVel = RandomVec(2, CIRCLE_XZ);
    e.randCol = RandomVec(0, 0.2, 0.3);
    e.randVel.lol = 2;
    e.v = vec3(0, 3, 0);
    e.life = 2;
    e.startAlpha = 1;
    e.endAlpha = 1;
    e.startCol = vec3(0.1, 0.2, 0.7);
    e.endCol = vec3(0, 0.1, 0.2);
    e.startSize = 0.7;
    e.endSize = 0;
    e.boom(1000);

    e.randVel = RandomVec(0.3, 5, 0.4);
    e.boom(1000);
    e.a = vec3(0, 0, 0);
    e.randVel = RandomVec(3, CIRCLE_XZ);
    e.v = vec3(0, 1, 0);
    e.boom(1000);

    sc->makeExplosion(p, 40.0, false, false, 6.0);

    alive = false;
}

void PlayerActor::render()
{
//    glColor3f(1, 1, 1);
//    playerTex->bind();
//    renderCube(p.x, p.y, p.z, size/2, size/2, size/2, body->GetAngle());
}

bool PlayerActor::renderParticle(Particle &p)
{
    p.p = this->p;
    p.startAlpha = 1;
    p.startSize = 2;
    p.startCol = vec3(0.4, 1.0, 0.0);
    p.isLight = true;

    return true;
}

void PlayerActor::collided(Actor *b)
{

    float dist = sc->GetRayCastDistance(b2Vec2(p.x, p.y), b2Vec2(p.x, p.y-size/2.0-0.5));
    canDash |= (dist < 1.0 && dist > 0.0);
    dist = sc->GetRayCastDistance(b2Vec2(p.x-size/2.0+0.02, p.y), b2Vec2(p.x-size/2.0+0.02, p.y-size/2.0-0.5));
    canDash |= (dist < 1.0 && dist > 0.0);
    dist = sc->GetRayCastDistance(b2Vec2(p.x+size/2.0-0.02, p.y), b2Vec2(p.x+size/2.0-0.02, p.y-size/2.0-0.5));
    canDash |= (dist < 1.0 && dist > 0.0);

    if(dynamic_cast<Enemy*>(b))
    {
        explode();
        return;
    }
    if (dynamic_cast<MagmaHexagon*>(b))
    {
        explode();
        return;
    }
    if (dynamic_cast<FireActor*>(b))
    {
        explode();
        return;
    }
}

bool PlayerActor::collidedWithGround()
{
    return false;
}
