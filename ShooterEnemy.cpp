#include "ShooterEnemy.h"
#include "FireActor.h"
#include "PlayerActor.h"
#include "GameScene.h"

ShooterEnemy::ShooterEnemy(GameScene* sc, vec3 pos) : Enemy(sc, 1.0, pos, vec3(0.0, 1.0, 0.0))
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
        shootTimer = 6 + frand(1);
        vec3 dir = player->p - p;
        normalize(dir);

        BulletConfig config;
        config.col1 = vec3(0.5, 0.5, 0.5);
        config.col2 = vec3(1, 0, 0);
        config.col3 = vec3(0, 1, 0);
        config.col4 = vec3(0, 0, 1);
        FireActor* bullet = new FireActor(p+dir*0.9f, dir*10.0f, gsc, &config);
        gsc->actors.push_back(bullet);
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x-dir.x, body->GetLinearVelocity().y-dir.y));
    }
}

void ShooterEnemy::render()
{
    Enemy::render();
}
