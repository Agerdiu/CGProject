#include "objloader.h"
#include "glut\glut.h"
#include <cstring>
#include <fstream>
ObjLoader::ObjLoader()
{
}

void ObjLoader::loadFromFile(char* objfile, char* mtlfile)
{
    loadMtlFile(mtlfile);
    loadObjFile(objfile);
    transToVAO();
}

void ObjLoader::loadMtlFile(char* filename)
{
	ifstream file;
	file.open(filename);
	string buf;
	if (!file)
	{
		cout << "Error: Mtl File cannot be opened!" << endl;
		exit(-2);
	}

    int index = -1; // 当前材质的下标
	string strList[100];
    while(getline(file,buf))
    {
		char* tmp = strtok((char*)buf.c_str(), " ");
        // 正式读取mtl
		int i = 0;
		while (tmp != NULL) {
			strList[i] = tmp;
			tmp = strtok(NULL, " ");
			i++;
		}
        if(strList[0] == "newmtl")
        {
            Material* tMtl = new Material();

            tMtl->mtlName = strList[1];
            m_mtls.push_back(tMtl);  // 获得一个新的材质

            index = m_mtls.size()-1;
        }
        if(strList[0] == "Ns")
        {
            m_mtls[index]->Ns = atof(strList[1].c_str());
        }
        if(strList[0] == "Ka")
        {
            m_mtls[index]->Ka[0] = atof(strList[1].c_str());
            m_mtls[index]->Ka[1] = atof(strList[2].c_str());
            m_mtls[index]->Ka[2] = atof(strList[3].c_str());
        }
        if(strList[0] == "Kd")
        {
			m_mtls[index]->Kd[0] = atof(strList[1].c_str());
			m_mtls[index]->Kd[1] = atof(strList[2].c_str());
			m_mtls[index]->Kd[2] = atof(strList[3].c_str());
        }
        if(strList[0] == "Ks")
        {
			m_mtls[index]->Ks[0] = atof(strList[1].c_str());
			m_mtls[index]->Ks[1] = atof(strList[2].c_str());
			m_mtls[index]->Ks[2] = atof(strList[3].c_str());
        }
		if (strList[0] == "map_Kd")
		{
			string imgPath = "./resources/objects/cube/" + strList[1];
			cout << imgPath<<endl;
			ifstream file_img;
			file_img.open(imgPath);
			if (!file_img)  // 看看纹理是否存在
			{
				cout << "texture doesn't exist!" << endl;
				continue;
			}
            m_mtls[index]->hasTexture = true;
            Texture::loadTexture((char*)imgPath.c_str(),&m_mtls[index]->texture);
        }
		if (strList[0] == "map_Ks")
		{
			string imgPath = "./resources/objects/cube/" + strList[1];
			//string imgPath = "./res/obj/car1/"+strList[1];
			cout << imgPath << endl;
			ifstream file_img;
			file_img.open(imgPath);
			if (!file_img)  // 看看纹理是否存在
			{
				cout << "texture doesn't exist!" << endl;
				continue;
			}
			m_mtls[index]->hasTexture = true;
			Texture::loadTexture((char*)imgPath.c_str(), &m_mtls[index]->texture);
		}
		if (strList[0] == "map_Ka")
		{
			string imgPath = "./resources/objects/cube/" + strList[1];
			//string imgPath = "./res/obj/car1/"+strList[1];
			cout << imgPath << endl;
			ifstream file_img;
			file_img.open(imgPath);
			if (!file_img)  // 看看纹理是否存在
			{
				cout << "texture doesn't exist!" << endl;
				continue;
			}
			m_mtls[index]->hasTexture = true;
			Texture::loadTexture((char*)imgPath.c_str(), &m_mtls[index]->texture);
		}
		if (strList[0] == "map_Bump")
		{
			string imgPath = "./resources/objects/cube/" + strList[1];
			//string imgPath = "./res/obj/car1/"+strList[1];
			cout << imgPath << endl;
			ifstream file_img;
			file_img.open(imgPath);
			if (!file_img)  // 看看纹理是否存在
			{
				cout << "texture doesn't exist!" << endl;
				continue;
			}
			m_mtls[index]->hasTexture = true;
			Texture::loadTexture((char*)imgPath.c_str(), &m_mtls[index]->texture);
		}
    }
	file.close();
}

void ObjLoader::loadObjFile(char* filename)
{
	ifstream file;
	file.open(filename);
	string buf;
	if (!file)
	{
		cout << "Error: Mtl File cannot be opened!" << endl;
		exit(-2);
	}
 
	string strList[100];
	int id = -1;// 当前材质的下标
    while(getline(file, buf))
    {
        // 正式开始读取
		char* tmp = strtok((char*)buf.c_str(), "/ ");
		// 正式读取mtl
		int count = 0;
		while (tmp!=NULL) {
			strList[count] = tmp;
			tmp = strtok(NULL, "/ ");
			count++;
		}
        if(strList[0] == "v")
        {
            Point3 tmp(atof(strList[1].c_str()), atof(strList[2].c_str()), atof(strList[3].c_str()));
            vertexs.push_back(tmp);
        }
        if(strList[0] == "vt")
        {
			Point3 tmp(atof(strList[1].c_str()), atof(strList[2].c_str()), 0.0);
			//cout << tmp.x << " " << tmp.y << " " << tmp.z << endl;
            textures.push_back(tmp);
        }
        if(strList[0] == "vn")
        {
			Point3 tmp(atof(strList[1].c_str()), atof(strList[2].c_str()), atof(strList[3].c_str()));
            normals.push_back(tmp);
        }

        if(strList[0] == "usemtl")
        {
            for(int i=0; i < m_mtls.size(); i++)  // 找到对应名称的材质
            {
                if( strList[1] == m_mtls[i]->mtlName)
                {
					cout << m_mtls[i]->mtlName << endl;
                    id = i;
					cout << id << endl;
                    break;
                }
            }
        }
        if(strList[0] == "f")
        {
			//cout << "count=" << count << " ";
            for(int i=0; i<count-1;i++)
            {
				//cout << atoi(strList[i + 1].c_str()) << "/";
                if(count == 13)
                    (m_mtls[id]->quadFaces).push_back(atoi(strList[i+1].c_str()));
                if(count == 10)
                    (m_mtls[id]->triFaces).push_back(atoi(strList[i+ 1].c_str()));
            }
			//cout << endl;
        }
    }
	file.close();
}

void ObjLoader::transToVAO()
{
    int sum = 0;
    for(int i=0; i<m_mtls.size();i++)
    {
        // 四角面和三角面的个数:
        m_mtls[i]->quadNum = m_mtls[i]->quadFaces.size()/12;
        m_mtls[i]->triNum = m_mtls[i]->triFaces.size()/9;

        m_mtls[i]->triVertexs = new GLdouble[m_mtls[i]->triNum*3*3];
        m_mtls[i]->triTextures = new GLdouble[m_mtls[i]->triNum*3*3];
        m_mtls[i]->triNormals = new GLdouble[m_mtls[i]->triNum*3*3];

        m_mtls[i]->quadVertexs = new GLdouble[m_mtls[i]->quadNum*4*3];
        m_mtls[i]->quadTextures = new GLdouble[m_mtls[i]->quadNum*4*3];
        m_mtls[i]->quadNormals = new GLdouble[m_mtls[i]->quadNum*4*3];

        sum+=(m_mtls[i]->triNum+m_mtls[i]->quadNum);
		cout << "triNum:" << m_mtls[i]->triNum << endl;
		cout << "quadNum:" << m_mtls[i]->quadNum << endl;

        for(int j=0; j<m_mtls[i]->triNum ; j++)  // 第几个三角面
        {
            for(int k=0; k<3;k++) // 第j个三角面的第k个点
            {
                m_mtls[i]->triVertexs[j*3*3+k*3+0] = vertexs[m_mtls[i]->triFaces[j*9+k*3+0]-1].x;
                m_mtls[i]->triVertexs[j*3*3+k*3+1] = vertexs[m_mtls[i]->triFaces[j*9+k*3+0]-1].y;
                m_mtls[i]->triVertexs[j*3*3+k*3+2] = vertexs[m_mtls[i]->triFaces[j*9+k*3+0]-1].z;

                m_mtls[i]->triTextures[j*3*3+k*3+0] = textures[m_mtls[i]->triFaces[j*9+k*3+1]-1].x;
                m_mtls[i]->triTextures[j*3*3+k*3+1] = textures[m_mtls[i]->triFaces[j*9+k*3+1]-1].y;
                m_mtls[i]->triTextures[j*3*3+k*3+2] = textures[m_mtls[i]->triFaces[j*9+k*3+1]-1].z;

                m_mtls[i]->triNormals[j*3*3+k*3+0] = normals[m_mtls[i]->triFaces[j*9+k*3+2]-1].x;
                m_mtls[i]->triNormals[j*3*3+k*3+1] = normals[m_mtls[i]->triFaces[j*9+k*3+2]-1].y;
                m_mtls[i]->triNormals[j*3*3+k*3+2] = normals[m_mtls[i]->triFaces[j*9+k*3+2]-1].z;

            }
        }

        for(int j=0; j<m_mtls[i]->quadNum ; j++)  // 第几个四角面
        {
            for(int k=0; k<4 ;k++) // 第j个三角面的第k个点
            {
                m_mtls[i]->quadVertexs[j*4*3+k*3+0] = vertexs[m_mtls[i]->quadFaces[j*12+k*3+0]-1].x;
                m_mtls[i]->quadVertexs[j*4*3+k*3+1] = vertexs[m_mtls[i]->quadFaces[j*12+k*3+0]-1].y;
                m_mtls[i]->quadVertexs[j*4*3+k*3+2] = vertexs[m_mtls[i]->quadFaces[j*12+k*3+0]-1].z;

                m_mtls[i]->quadTextures[j*4*3+k*3+0] = textures[m_mtls[i]->quadFaces[j*12+k*3+1]-1].x;
                m_mtls[i]->quadTextures[j*4*3+k*3+1] = textures[m_mtls[i]->quadFaces[j*12+k*3+1]-1].y;
                m_mtls[i]->quadTextures[j*4*3+k*3+2] = textures[m_mtls[i]->quadFaces[j*9+k*3+1]-1].z;


                m_mtls[i]->quadNormals[j*4*3+k*3+0] = normals[m_mtls[i]->quadFaces[j*12+k*3+2]-1].x;
                m_mtls[i]->quadNormals[j*4*3+k*3+1] = normals[m_mtls[i]->quadFaces[j*12+k*3+2]-1].y;
                m_mtls[i]->quadNormals[j*4*3+k*3+2] = normals[m_mtls[i]->quadFaces[j*12+k*3+2]-1].z;

            }
        }
    }
    cout<<"Transfer VAO finish!"<<endl;
}

void ObjLoader::renderObj()
{
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );
	cout << "render is finished" << endl;
    for(int i=0; i<m_mtls.size();i++)
    {
        if(m_mtls[i]->hasTexture)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,m_mtls[i]->texture);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            //glColor3f(1.0f,1.0f,1.0f);
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            //glColor3fv(m_mtls[i]->Kd);
        }

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_mtls[i]->Ka);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_mtls[i]->Kd);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_mtls[i]->Ks);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_mtls[i]->Ns);



        glVertexPointer(3,GL_DOUBLE,0,m_mtls[i]->triVertexs);
        glNormalPointer(GL_DOUBLE,0,m_mtls[i]->triNormals);
        glTexCoordPointer(3,GL_DOUBLE,0,m_mtls[i]->triTextures);

        glDrawArrays(GL_TRIANGLES,0,m_mtls[i]->triNum*3);

        glVertexPointer(3,GL_DOUBLE,0,m_mtls[i]->quadVertexs);
        glNormalPointer(GL_DOUBLE,0,m_mtls[i]->quadNormals);
        glTexCoordPointer(3,GL_DOUBLE,0,m_mtls[i]->quadTextures);
        glDrawArrays(GL_QUADS,0,m_mtls[i]->quadNum*4);

    }
	glutSwapBuffers();
	glFlush();
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

}
