#include "Level.h"
#include "GameScene.h"

Level::Level(GameScene* sc)
{
    gsc = sc;
}

Level::Level(GameScene* sc, const string name)
{
    gsc = sc;
    this->name = name;
}

Level::Level(GameScene* sc, const string name, const string filename, const list<Asset>& assets)
{
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

void Level::Setup(const string name, const string filename, const list<Asset> &assets)
{
    this->name = name;
    this->filename = filename;
    this->assets = assets;
}

void Level::Build()
{

}
