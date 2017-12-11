#pragma once
#ifndef __opengl__Particle__
#define __opengl__Particle__
#include "CBMPLoader.h"
#include "glut\glut.h"
#include <iostream>
#include <vector>
using namespace std;
/** 粒子结构 */
struct Particle
{
	float x, y, z;            /**< 粒子的位置 */
	unsigned int  r, g, b;    /**< 粒子的颜色 */
	float vx, vy, vz;         /**< 粒子的速度(x,y,z方向) */
	float ax, ay, az;         /**< 粒子在x，y，z上的加速度 */
	float lifetime;         /**< 粒子生命值 */
	float size=0.002;             /**< 粒子尺寸 */
	float dec;              /**< 粒子消失的速度 */
};

/** 粒子类 */
class CParticle
{
private:
	vector<Particle>   data;               /**< 粒子指针 */
	CBMPLoader loader;
public:
	CParticle();          /**< 构造函数 */
	~CParticle();         /**< 析构函数 */

						  /** 创建粒子数组 */
	int Create(long num);

	/** 设置和获取颜色属性 */
	int SetColor(GLint r, GLint g, GLint b);
	int SetColor(GLint index, GLint r, GLint g, GLint b);
	int GetColor(GLint index, GLint &r, GLint &g, GLint &b);

	/** 设置和获取速度属性 */
	int SetVelocity(GLfloat vx, GLfloat vy, GLfloat vz);
	int SetVelocity(GLint index, GLfloat vx, GLfloat vy, GLfloat vz);
	int GetVelocity(GLint index, GLfloat &vx, GLfloat &vy, GLfloat &vz);

	/** 设置和获取位置属性 */
	int SetPosition(GLfloat x, GLfloat y, GLfloat z);
	int SetPosition(GLint index, GLfloat x, GLfloat y, GLfloat z);
	int GetPosition(GLint index, GLfloat &x, GLfloat &y, GLfloat &z);

	/** 设置和获取加速度属性 */
	int SetAcceleration(GLfloat ax, GLfloat ay, GLfloat az);
	int SetAcceleration(GLint index, GLfloat ax, GLfloat ay, GLfloat az);
	int GetAcceletation(GLint index, GLfloat &ax, GLfloat &ay, GLfloat &az);

	/** 设置和获取尺寸属性 */
	int SetSize(GLfloat size);
	int SetSize(GLint index, GLfloat size);
	int GetSize(GLint index, GLfloat &size);

	/** 设置和获取消失速度属性 */
	int SetDec(GLfloat dec);
	int SetDec(GLint index, GLfloat dec);
	int GetDec(GLint index, GLfloat &dec);

	/** 设置和获取生命值属性 */
	int SetLifeTime(GLfloat lifetime);
	int SetLifeTime(GLint index, GLfloat lifetime);
	int GetLifeTime(GLint index, GLfloat &lifetime);

	/** 获取粒子数组地址 */
	//Particle GetParticle() { return data; }

	/** 获得粒子的数目 */
	int GetNumOfParticle() { return data.size(); }

	/** 获得粒子所有的属性 */
	int GetAll(int index,                               /**< 索引 */
		GLint &r, GLint &g, GLint &b,              /**< 粒子的颜色 */
		GLfloat &x, GLfloat &y, GLfloat &z,        /**< 位置 */
		GLfloat &vx, GLfloat &vy, GLfloat &vz, /**< 速度 */
		GLfloat &ax, GLfloat &ay, GLfloat &az, /**< 加速度 */
		GLfloat &size,                           /**< 大小 */
		GLfloat &lifetime,                       /**< 生命时间 */
		GLfloat &dec                         /**< 消失速度 */
	);

	/** 设置粒子的所有属性 */
	int SetAll(int index,                           /**< 索引 */
		GLint r, GLint g, GLint b,         /**< 粒子的颜色 */
		GLfloat x, GLfloat y, GLfloat z,       /**< 位置 */
		GLfloat vx, GLfloat vy, GLfloat vz,    /**< 速度 */
		GLfloat ax, GLfloat ay, GLfloat az,    /**< 加速度 */
		GLfloat size,                        /**< 大小 */
		GLfloat lifetime,                    /**< 生命时间 */
		GLfloat dec                          /**< 消失速度 */
	);
	bool LoadTextures(char*filename);
	void Draw();
	void Update(int num);
	bool Init();
};

#endif /* defined(__opengl__Particle__) */