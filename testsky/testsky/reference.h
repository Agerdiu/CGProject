#ifndef REFERENCE_H
#define REFERENCE_H
#include <windows.h> 
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>
#include "CBMPLoader.h"
#define PAI 3.1415926
#define GL_CLAMP_TO_EDGE    0x812F
using namespace std;
class Point2
{
    double x,y;

    Point2():x(0),y(0){}
    Point2(double x,double y):x(x),y(y){}
};

class Point3
{
public:
    double x,y,z;
    Point3():x(0),y(0),z(0){}
	Point3(double x1,double y1,double z1) :x(x1), y(y1), z(z1) {}
  
};

class Triangle {
public:
	Point3 x, y, z;
	Triangle(){};
	Triangle(Point3 x1, Point3 y1, Point3 z1) :x(x1), y(y1), z(z1) {}
	Triangle(double x1, double y1, double z1,
		double x2, double y2, double z2,
		double x3, double y3, double z3) :x(x1, x2, x3), y(y1, y2, y3), z(z1, z2, z3) {}
};

class Texture 
{
public:
    static bool loadTexture(char* filepath, CBMPLoader *loader)
    {

        if(!loader->LoadBitmap(filepath))
        {
            cout<<"Error: failed to load image!"<<endl;
            //exit(1);
			return false;
        }

        //tex = convertToGLFormat(buf);
        glGenTextures(1, &(loader->ID));
        glBindTexture(GL_TEXTURE_2D, (loader->ID));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, loader->imageWidth, loader->imageHeight, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)loader->image);
		return true;
    }
};

class MyMath
{
public:
    static double mSin(double a){
        return sin(PAI*a/180.0);
    }

    static double mCos(double a){
        return cos(PAI*a/180.0);
    }
};

class Vec2{
public:
    double x;
    double y;

    Vec2():x(0),y(0){}
    Vec2(double x,double y):x(x),y(y){}
};

class Vec3{
public:
    double x,y,z;

    Vec3():x(0),y(0),z(0){}
    Vec3(double x,double y,double z):x(x),y(y),z(z){}

    Vec3 operator +(const Vec3& v)
    {
        Vec3 res;

        res.x = x+v.x;
        res.y = y+v.y;
        res.z = z+v.z;

        return res;
    }

    Vec3 operator -(const Vec3& v)
    {
        Vec3 res;

        res.x = x-v.x;
        res.y = y-v.y;
        res.z = z-v.z;

        return res;
    }
};

class Vector
{
public:
    static Point3 normalize(Point3& a, Point3& b, Point3& c)
    {

        //  A = (ba);   B=(bc)  计算A×B
        Point3 normal, A, B;

        A = Point3(a.x-b.x,a.y-b.y,a.z-b.z);
        B = Point3(c.x-b.x,c.y-b.y,c.z-b.z);

        normal.x = A.y*B.z-A.z*B.y;
        normal.y = A.z*B.x-A.x*B.z;
        normal.z = A.x*B.y-A.y*B.x;

        return normal;
    }

    static Vec3 unit(const Vec3& v) // 得到单位向量
    {
        double mod = sqrt(v.x*v.x+v.y*v.y+v.z*v.z);

        return Vec3(v.x/mod,v.y/mod,v.z/mod);
    }

    static Vec3 multiple(const Vec3& v,const double& n) //数乘向量
    {
        return Vec3(v.x*n,v.y*n,v.z*n);
    }

    static Vec3 opposite(const Vec3& v)  // 向量反向
    {
        return Vec3(-v.x,-v.y,-v.z);
    }

    static double getMod(const Vec3& v)
    {
        return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
    }
};

#endif // REFERENCE_H
