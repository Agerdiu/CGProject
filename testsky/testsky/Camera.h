#pragma once
//
//  Camera.h
//  GLUTTest
//
//  Created by C-TEN on 2017/10/22.
//  Copyright © 2017年 C-TEN. All rights reserved.
//

#ifndef Camera_h
#define Camera_h


#include <windows.h>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include	"glut\glut.h"
//#include <OpenGL/OpenGL.h>
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
//#include <OpenGL/gl3.h>
#include "Vector.h"                 /**< 包含向量类头文件 */
#include "bullet.h"
#include <vector>
#include <iostream>
using namespace std;
/** 摄像机类 */
class Camera
{
public:
	/** 构造函数和析构函数 */
	Camera();
	~Camera();

	/** 获得摄像机状态方法 */
	Vector3 getPosition() { return m_Position; }
	Vector3 getView() { return m_View; }
	Vector3 getUpVector() { return m_UpVector; }
	float   getSpeed() { return m_Speed; }

	/** 设置速度 */
	void    setSpeed(float speed)
	{
		m_Speed = speed;
	}

	/** 设置摄像机的位置, 观察点和向上向量 */
	void setCamera(float positionX, float positionY, float positionZ,
		float viewX, float viewY, float viewZ,
		float upVectorX, float upVectorY, float upVectorZ);

	/** 旋转摄像机方向 */
	void rotateView(float angle, float X, float Y, float Z);

	/** 根据鼠标设置摄像机观察方向 */
	void setViewByMouse(int x,int y);

	/** 左右摄像机移动 */
	void yawCamera(float speed);

	/** 前后移动摄像机 */
	void moveCamera(float speed);
	void goCamera(float speed);

	/** 放置摄像机 */
	void setLook();

//private:
	/** 摄像机属性 */
	Vector3        m_Position;      /**< 位置 */
	Vector3        m_View;          /**< 朝向 */
	Vector3        m_UpVector;      /**< 向上向量 */
	float          m_Speed;         /**< 速度 */

};
class Shooter:public Camera {
public:
	vector<Bullet> bulletList;
	Model gun;
	~Shooter() { bulletList.clear(); }
	void shot() {
		vector<Bullet>::iterator iter;
		for (iter = bulletList.begin(); iter!=bulletList.end();) {
			if ((*iter).duration < 1000) {
				//glColor3f(0.5f, 0.25f, 0.0f);
				(*iter).shot();
				iter++;
			}
			else
			iter = bulletList.erase(iter);
		}
	}
	void renderGun() {
		gun.direction = Vector3(m_View.x- m_Position.x, m_View.y - m_Position.y, m_View.z - m_Position.z);
		gun.position=Vector3(m_Position.x + (m_View.x - m_Position.x)*0.5, m_Position.y + (m_View.y - m_Position.y)*0.5,
			m_Position.z+ (m_View.z - m_Position.z)*0.5);
		gun.render();
	}
	bool is_injured(Model* model) {
		if (model->is_In_Model(Point3(m_Position.x,m_Position.y,m_Position.z))&&model->visible) {
			model->visible = false;
			cout << "You are injured" << endl;
			return true;
		}
		return false;
	}
	bool is_hit(Model* model) {
		vector<Bullet>::iterator iter;
		for (iter = bulletList.begin(); iter != bulletList.end();) {
			if ((*iter).is_hit(model)) {
				iter = bulletList.erase(iter);
				cout << "hit" << endl;
				return true;
			}else
				++iter;
		}
		return false;
	}
	void addBullet() {
		Bullet bullet(Point3(m_Position.x, m_Position.y,m_Position.z),
			Vec3(m_View.x- m_Position.x, m_View.y- m_Position.y, m_View.z- m_Position.z));
		//Bullet bullet(Point3(m_Position.x, m_Position.y, m_Position.z),
			//Vec3(- m_Position.x, - m_Position.y, - m_Position.z));
		//Bullet bullet(Point3(m_Position.x, m_Position.y, m_Position.z), Vec3(0,0,1));
		//cout << m_Position.x << " " << m_Position.y << " "<<m_Position.z << endl;
		//cout << m_View.x << " " << m_View.y << " " << m_View.z << endl;
		bulletList.push_back(bullet);
	}
	void Draw_aim() {

		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
		glPushMatrix();               // 保存原矩阵
		glLoadIdentity();             // 装入单位矩阵
		glOrtho(0, 480, 0, 480, -1, 1);    // 位置正投影
		glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
		glPushMatrix();               // 保存原矩阵
		glLoadIdentity();             // 装入单位矩阵
		glLineWidth(3);
		glColor3f(0,1, 0);
		glBegin(GL_LINES);
		glVertex2i(240, 250);
		glVertex2i(260, 250);
		glVertex2i(250, 235);
		glVertex2i(250, 265);
		glEnd();
		glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
		glPopMatrix();                // 重置为原保存矩阵
		glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
		glPopMatrix();              
		// 重置为原保存矩阵
		glEnable(GL_DEPTH_TEST);
		glColor3f(1, 1, 1);
	}
};

#endif /* Camera_h */
