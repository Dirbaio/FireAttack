#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <list>
#include "util.h"
#include "PlayerActor.h"

using namespace std;

enum Type
{
    PLAYER1,
    PLAYER2,
    PLAYER3,
    PLAYER4,
    HEXFLOATING,
    HEXSOLID,
    HEXSTATIC,
    HEXEXPLOSIVE,
    HEXBOUNCY,
    HEXMAGMA,
    HEXTRAP,
    ENEMSTANDARD,
    ENEMSHOOTER,
    ENEMSTICKY
};

class GameScene;

struct Asset
{
    Type type;
    vec3 pos;
    Asset(Type t, vec3 p)
    {
        type = t;
        pos = p;
    }
};

class Level
{
private:
    string name;
    string filename;
    GameScene* gsc;
    list<Asset> assets;
    PlayerConfig player1, player2;

public:
    Level(GameScene* sc);
    Level(GameScene* sc, const string name);
    Level(GameScene* sc, const string name, const string filename, const list<Asset>& assets);

    const string getName();
    const string getFilename();

    void setName(const string name);
    void setFilename(const string filename);

    void Setup(const string name, const string filename, const list<Asset>& assets);
    void Build();
    void AddAsset(const Asset& asset, bool build);

    void BuildAsset(const Asset& asset);
};

#endif // LEVEL_H
