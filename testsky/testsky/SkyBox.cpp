//
//  SkyBox.cpp
//  GLUTTest
//
//  Created by C-TEN on 2017/10/22.
//  Copyright © 2017年 C-TEN. All rights reserved.
//

#include "SkyBox.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
float fTranslate;
float fRotate;
float fScale;

void DrawCube(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, GLfloat z1, GLfloat z2) {
	int i, j;
	GLfloat vertex[8][3] = {
		x1, y1, z1, // [6 5]
		x1, y2, z1, // [7 4]
		x2, y2, z1, // [1 2]
		x2, y1, z1, // [0 3]
		x2, y1, z2,
		x2, y2, z2,
		x1, y2, z2,
		x1, y1, z2,
	};
	GLint surface[6][4] = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		2, 3, 4, 5,
		1, 2, 5, 6,
		0, 1, 6, 7,
		0, 3, 7, 4,
	};
	glBegin(GL_QUADS);
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 4; j++) {
			//glEnable(GL_TEXTURE_2D);
			glVertex3fv(vertex[surface[i][j]]);
		}
	}
	glEnd();
}

void DrawDesk()
{
	DrawCube(0.0, 1.0, 0.0, 0.8, 0.6, 0.8);

}
void DrawLen() {
	DrawCube(0.1, 0.3, 0.1, 0.3, 0.0, 0.6);
	DrawCube(0.7, 0.9, 0.1, 0.3, 0.0, 0.6);
	DrawCube(0.1, 0.3, 0.5, 0.7, 0.0, 0.6);
	DrawCube(0.7, 0.9, 0.5, 0.7, 0.0, 0.6);
}



CSkyBox::CSkyBox()
{
	//cout << "构造skybox" << endl;
}

CSkyBox::~CSkyBox()
{
	/** 删除纹理对象及其占用的内存 */
	for (int i = 0; i< 6; i++)
	{
		m_texture[i].FreeImage();
		glDeleteTextures(1, &m_texture[i].ID);
	}

}

/** 天空盒初始化 */
bool CSkyBox::Init()
{
	char filename[128];                                         /**< 用来保存文件名 */	//char *bmpName[] = { "desertsky_back","desertsky_front","bottom","desertsky_top","desertsky_left","desertsky_right" };
	char *bmpName[] = { "back","front","bottom","top","left","right" };
	for (int i = 0; i< 6; i++)
	{
		//sprintf(filename, "C:/Users/luyujie.CN/source/repos/testsky/testsky/data/%s", bmpName[i]);
		//strcat(filename, ".bmp");
		sprintf(filename, "F:/图形学/专题研讨lab/专题研讨lab/testsky/testsky/testsky/resources/textures/skybox/%s", bmpName[i]);
		strcat(filename, ".bmp");
		if (!m_texture[i].LoadBitmap(filename))                     /**< 载入位图文件 */
		{
			cout << filename <<"载入失败" << endl;
			 //           MessageBox(NULL, (LPCWSTR)"装载位图文件失败！", (LPCWSTR)"错误", MB_OK);    /**< 如果载入失败则弹出对话框 */
			  //          exit(0);
		}
		glGenTextures(1, &m_texture[i].ID);                        /**< 生成一个纹理对象名称 */

		glBindTexture(GL_TEXTURE_2D, m_texture[i].ID);             /**< 创建纹理对象 */
																   /** 控制滤波: */
																   /*
																   其中GL_TEXTURE_WRAP_S，GL_TEXTURE_WRAP_T通常可设置为GL_REPEAT或GL_CLAMP两种方式。
																   当待填充的多边形大于纹理的时候，GL_REPEAT表示多余的部分用重复的方式填充；GL_CLAMP表示多余的部分用相连边缘的相邻像素填充。
																   在实际绘制中，我们一般采用GL_CLAMP_EDGE来处理，这就消除了接缝处的细线，增强了天空盒的真实感。
																   */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		/** 创建纹理 */
		        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_texture[i].imageWidth,
		                          m_texture[i].imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
		                          m_texture[i].image);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_texture[i].imageWidth, m_texture[i].imageHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_texture[i].image);
	}
	return true;

}
/** 构造天空盒 */
void  CSkyBox::CreateSkyBox(float x, float y, float z,
	float box_width, float box_height,
	float box_length)
{
	/** 获得场景中光照状态 */
	glColor3f(1, 1, 1);
	GLboolean lp;
	glGetBooleanv(GL_LIGHTING, &lp);

	/** 计算天空盒长 宽 高 */
	float width = MAP * box_width / 8;
	float height = MAP * box_height / 8;
	float length = MAP * box_length / 8;

	/** 计算天空盒中心位置 */
	x = x + MAP / 8 - width / 2;
	y = y + MAP / 8 - height / 2;
	z = z + MAP / 8 - length / 2;

	glDisable(GL_LIGHTING);            /**< 关闭光照 */

									   /** 开始绘制 */
	glPushMatrix();
	glTranslatef(-x, -y, -z);

	/** 绘制背面 */
	glBindTexture(GL_TEXTURE_2D, m_texture[0].ID);

	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);

	glEnd();

	/** 绘制前面 */
	glBindTexture(GL_TEXTURE_2D, m_texture[1].ID);

	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);

	glEnd();

	/** 绘制底面 */
	glBindTexture(GL_TEXTURE_2D, m_texture[2].ID);

	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);

	glEnd();

	/** 绘制顶面 */
	glBindTexture(GL_TEXTURE_2D, m_texture[3].ID);

	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);

	glEnd();

	/** 绘制左面 */
	glBindTexture(GL_TEXTURE_2D, m_texture[4].ID);

	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);

	glEnd();

	/** 绘制右面 */
	glBindTexture(GL_TEXTURE_2D, m_texture[5].ID);

	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	glPopMatrix();                 /** 绘制结束 */


	if (lp)                         /** 恢复光照状态 */
		glEnable(GL_LIGHTING);

}