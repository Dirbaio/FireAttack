#ifndef MODEL_H
#define MODEL_H
#include "util.h"
#include<vector>

class Model
{
public:

    vector<vec3> normals;
    vector<vec3> vertices;
    vector<vec3> texcoords;
    vector<vec3> normalArray;
    vector<vec3> vertexArray;
    vector<vec3> texcoordArray;

    Model();
    Model(string filename);
    void draw();
    void CreateModel(string filename);
};

#endif // MODEL_H
