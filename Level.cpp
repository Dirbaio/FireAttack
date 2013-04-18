#include "Level.h"
#include "GameScene.h"
#include "FloatingHexagon.h"
#include "SolidHexagon.h"
#include "StaticHexagon.h"
#include "BouncyHexagon.h"
#include "ExplosiveHexagon.h"
#include "MagmaHexagon.h"
#include "TrapHexagon.h"
#include "StandardEnemy.h"
#include "ShooterEnemy.h"
#include "StickyEnemy.h"

void configPlayers(PlayerConfig& player1, PlayerConfig& player2)
{
    player1.col1 = vec3(1.0, 1.0, 0.5);
    player1.col2 = vec3(1.0, 1.0, 1.0);
    player1.col3 = vec3(1.0, 0.0, 0.0);
    player1.col4 = vec3(1.0, 1.0, 0.0);
    player1.col5 = vec3(1.0, 0.0, 0.0);
    player1.col6 = vec3(0.0, 1.0, 0.4);

    player2.col1 = vec3(0.5, 1.0, 1.0);
    player2.col2 = vec3(0.1, 1.0, 1.0);
    player2.col3 = vec3(0.0, 0.0, 1.0);
    player2.col4 = vec3(0.0, 1.0, 1.0);
    player2.col5 = vec3(0.0, 0.0, 1.0);
    player2.col6 = vec3(0.4, 1.0, 0.0);

    player1.initPos = vec3(0, 10, 0);
    player2.initPos = vec3(4, 10, 0);

    player1.keyMap.resize(MAPPINGSIZE);
    player2.keyMap.resize(MAPPINGSIZE);

    player1.keyMap[JUMP] = sf::Keyboard::Space;
    player1.keyMap[MOVERIGHT] = sf::Keyboard::D;
    player1.keyMap[MOVELEFT] = sf::Keyboard::A;
    player1.keyMap[SHOOT] = sf::Keyboard::LShift;
    player1.keyMap[SPAWN] = sf::Keyboard::E;

    player2.keyMap[JUMP] = sf::Keyboard::RShift;
    player2.keyMap[MOVERIGHT] = sf::Keyboard::L;
    player2.keyMap[MOVELEFT] = sf::Keyboard::J;
    player2.keyMap[SHOOT] = sf::Keyboard::Return;
    player2.keyMap[SPAWN] = sf::Keyboard::O;

    player1.useWiimote = false;
    player2.useWiimote = true;

    player1.numWiimote = 1;
    player2.numWiimote = 0;

}

void Level::BuildAsset(const Asset& asset)
{
    switch (asset.type)
    {
    case PLAYER1:
        gsc->actors.push_back(new PlayerActor(gsc, &player1, 0));
        break;
    case PLAYER2:
        gsc->actors.push_back(new PlayerActor(gsc, &player2, 1));
        break;
    case PLAYER3:
        gsc->actors.push_back(new PlayerActor(gsc, &player1, 2));
        break;
    case PLAYER4:
        gsc->actors.push_back(new PlayerActor(gsc, &player2, 3));
        break;
    case HEXFLOATING:
        gsc->actors.push_back(new FloatingHexagon(gsc, asset.pos));
        break;
    case HEXSOLID:
        gsc->actors.push_back(new SolidHexagon(gsc, asset.pos));
        break;
    case HEXSTATIC:
        gsc->actors.push_back(new StaticHexagon(gsc, asset.pos));
        break;
    case HEXEXPLOSIVE:
        gsc->actors.push_back(new ExplosiveHexagon(gsc, asset.pos));
        break;
    case HEXBOUNCY:
        gsc->actors.push_back(new BouncyHexagon(gsc, asset.pos));
        break;
    case HEXMAGMA:
        gsc->actors.push_back(new MagmaHexagon(gsc, asset.pos));
        break;
    case HEXTRAP:
        gsc->actors.push_back(new TrapHexagon(gsc, asset.pos));
        break;
    case ENEMSTANDARD:
        gsc->actors.push_back(new StandardEnemy(gsc, asset.pos));
        break;
    case ENEMSHOOTER:
        gsc->actors.push_back(new ShooterEnemy(gsc, asset.pos));
        break;
    case ENEMSTICKY:
        gsc->actors.push_back(new StickyEnemy(gsc, asset.pos));
        break;
    default:
        cerr << "Asset not found" << endl;
    }
}

Level::Level(GameScene* sc)
{
    configPlayers(player1, player2);
    gsc = sc;
}

Level::Level(GameScene* sc, const string name)
{
    configPlayers(player1, player2);
    gsc = sc;
    this->name = name;
}

Level::Level(GameScene* sc, const string name, const string filename, const list<Asset>& assets)
{
    configPlayers(player1, player2);
    gsc = sc;
    this->name = name;
    this->filename = filename;
    this->assets = assets;
}

const string Level::getName()
{
    return name;
}

const string Level::getFilename()
{
    return filename;
}

void Level::setName(const string name)
{
    this->name = name;
}

void Level::setFilename(const string filename)
{
    this->filename = filename;
}

void Level::Setup(const string name, const string filename, const list<Asset> &assets)
{
    this->name = name;
    this->filename = filename;
    this->assets = assets;
}

void Level::Build()
{
    for (list<Asset>::iterator it = assets.begin(); it != assets.end(); it++)
    {
        BuildAsset(*it);
    }
}

void Level::AddAsset(const Asset &asset, bool build)
{
    assets.push_back(asset);
    if (build)
    {
        BuildAsset(asset);
    }
}
