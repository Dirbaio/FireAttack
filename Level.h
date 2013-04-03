#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <list>

using namespace std;

enum Type
{

};

class GameScene;

struct Param
{

};

struct Asset
{
    Type type;
};

class Level
{
private:
    string name;
    string filename;
    GameScene* gsc;
    list<Asset> assets;

public:
    Level(GameScene* sc);
    Level(GameScene* sc, const string name);
    Level(GameScene* sc, const string name, const string filename, const list<Asset>& assets);

    const string getName();
    const string getFilename();

    void Setup(const string name, const string filename, const list<Asset>& assets);
    void Build();
};

#endif // LEVEL_H
