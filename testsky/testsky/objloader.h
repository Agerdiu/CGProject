#ifndef OBJLOADER_H
#define OBJLOADER_H
#include "reference.h"
#include "CBMPLoader.h"
#include <vector>
#include <string>
class Material  // 材质下面管理一系列面
{
public:
    string mtlName;

    double Ns;
    float Ka[3];
    float Kd[3];
    float Ks[3];

    bool hasTexture;
    GLuint texture; // 材料对应的纹理
	CBMPLoader loader;
    vector<int> triFaces;  // 三个一组组成面
    vector<int> quadFaces; // 四个一组组成面

    // vertex array object;
    int quadNum;  // 四角面个数
    int triNum;   // 三角面个数

    GLdouble *triVertexs;
    GLdouble *triNormals;
    GLdouble *triTextures;

    GLdouble *quadVertexs;
    GLdouble *quadNormals;
    GLdouble *quadTextures;

    Material():hasTexture(false){}
};

class ObjLoader
{
public:
    ObjLoader();

    void loadFromFile(char* objfile,char* mtlfile);
    void renderObj();

    // 临时数据 可以后期释放
    vector<Point3> vertexs;
    vector<Point3> textures;
    vector<Point3> normals;

    vector<Material*> m_mtls;
	
    void loadObjFile(char* filename);
    void loadMtlFile(char* filename);
    void transToVAO();
};

#endif // OBJLOADER_H
