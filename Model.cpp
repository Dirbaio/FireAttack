#include "Model.h"
#include<fstream>
#include<sstream>

Model::Model(string filename)
{
    CreateModel(filename);
}

void Model::CreateModel(string filename)
{
    ifstream in(filename.c_str(), ios::in);
    if (!in)
    {
        cerr << "Cannot open " << filename << endl;
        exit(1);
    }

    int faceCount = 0;
    string line;
    while (getline(in, line))
    {
        if (line.substr(0,2) == "v ")
        {
            istringstream s(line.substr(2));
            vec3 v;
            s >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
        else if (line.substr(0,3) == "vn ")
        {
            istringstream s(line.substr(3));
            vec3 v;
            s >> v.x >> v.y >> v.z;
            normals.push_back(v);
        }
        else if (line.substr(0,2) == "f ")
        {
            istringstream s(line.substr(2));
            char basura;
            int v1, n1, v2, n2, v3, n3;
            int t1, t2, t3;
            s >> v1 >> basura >> t1 >> basura >> n1
              >> v2 >> basura >> t2 >> basura >> n2
              >> v3 >> basura >> t3 >> basura >> n3;

            vertexArray.push_back(vertices[v1-1]);
            vertexArray.push_back(vertices[v2-1]);
            vertexArray.push_back(vertices[v3-1]);
            normalArray.push_back(normals[n1-1]);
            normalArray.push_back(normals[n2-1]);
            normalArray.push_back(normals[n3-1]);
            faceCount++;
        }
    }
//    cout<<"FACES: "<<faceCount<<endl;
}

void Model::draw()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, &vertexArray[0]);
    glNormalPointer(GL_FLOAT, 0, &normalArray[0]);

    glDrawArrays(GL_TRIANGLES, 0, vertexArray.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

