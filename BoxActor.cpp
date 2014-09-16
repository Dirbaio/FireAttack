#include "BoxActor.h"
#include "GameScene.h"
#include "PlayerActor.h"

BoxActor::BoxActor(GameScene* sc, bool op) : Actor(sc)
{
    this->gsc = sc;
    this->op = op;

    p.x = frand(10);
    p.y = frand(2)+15;
    size = 0.8+frand(0.2);

    b2BodyDef bodyDef;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.type = b2_dynamicBody;
    body = sc->world.CreateBody(&bodyDef);
    body->SetLinearDamping(3);
    b2PolygonShape box;
    box.SetAsBox(size/2, size/2);
    b2FixtureDef fixture;
    fixture.density = 1.0f;
    fixture.friction = 0.1f;
    fixture.shape = &box;
    fixture.restitution = 0.6f;
    fixture.userData = this;
    body->CreateFixture(&fixture);

    ParticleEmitter pe (this);
    pe.period = 1/600.0;
    pe.startAlpha = 0.1;
    pe.randPos = RandomVec(size/4);
    pe.startSize = size/3;
    pe.endSize = size/16;
    pe.life = 0.01;
    pe.startCol = vec3(1, 1, 0.5);
    pe.endCol = vec3(1, 1, 1);
    pe.actorVelMult = 1;
    emitters.push_back(pe);

    pe.period = 1/1400.0;
    pe.randPos = RandomVec(size/2);
    pe.startSize = size/3;
    pe.endSize = size/16;
    pe.life = 0.4;
    pe.startAlpha = 0.1;
    pe.startCol = !op?vec3(0, 0, 1):vec3(0, 1, 0);
    pe.endCol = vec3(0, 1, 1);
    pe.actorVelMult = -0.3;
    emitters.push_back(pe);
    shootTimer = 1;

    body->SetUserData(this);

}

void BoxActor::update()
{
    PlayerActor* pl = gsc->getPlayer();
    if(!pl) return;

    body->ApplyForceToCenter(b2Vec2(pl->p.x - p.x, 0), true);
    if(op)
    {
        shootTimer -= dt;
        if(shootTimer < 0)
        {
            shootTimer = 2 + frand(1);
            vec3 dir = pl->p - p;
            normalize(dir);

            FireActor* bullet = new FireActor(p+dir*0.9f, dir*10.0f, gsc, true);
            sc->actors.push_back(bullet);
            body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x-dir.x, body->GetLinearVelocity().y-dir.y));
        }
    }
}

void BoxActor::render()
{
//    glColor3f(1, 1, 1);
//    playerTex->bind();
//    renderCube(p.x, p.y, p.z, size/2, size/2, size/2, body->GetAngle());
}
