#include "ShooterEnemy.h"
#include "FireActor.h"
#include "PlayerActor.h"
#include "GameScene.h"

ShooterEnemy::ShooterEnemy(GameScene* sc, float size, vec3 pos, vec3 col) : Enemy(sc, size, pos, col)
{
    shootTimer = 1;
}

void ShooterEnemy::update()
{
    Enemy::update();

    if(!player) return;
    body->ApplyForceToCenter(b2Vec2(player->p.x - p.x, 0));

    shootTimer -= dt;
    if(shootTimer < 0)
    {
        shootTimer = 2 + frand(1);
        vec3 dir = player->p - p;
        normalize(dir);

        FireActor* bullet = new FireActor(p+dir*0.9f, dir*10.0f, gsc, true);
        gsc->actors.push_back(bullet);
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x-dir.x, body->GetLinearVelocity().y-dir.y));
    }
}

void ShooterEnemy::render()
{
    Enemy::render();
}
