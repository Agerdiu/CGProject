#include <iostream>
#include "glut\glut.h"
//#include <GLUT/glu.h>
//#include <GLUT/gl.h>
#include "GLAUX\GLAUX.H"
#include "Camera.h"
#include "SkyBox.h"
#include "objloader.h"
#include"glm.h"
#include "Model.h"
#include "bullet.h"
#include <vector>
#include <cmath>
#include "Map.h"
#include "MS3D.h"
using namespace std;
//const unsigned int MAP_WIDTH = 1024;
//const unsigned int CELL_WIDTH = 16;
//const unsigned int MAP = MAP_WIDTH * CELL_WIDTH / 2;
GLUquadricObj *g_text = gluNewQuadric();
Shooter m_Shooter;
CSkyBox m_SkyBox;
boolean roam = false;
GLint lid;
float duration = 0.1f;
float durationYaw = 0.1f;
int flag = 0;
int selectCnt = 0;
//define the eyepoint
typedef struct EyePoint
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
}EyePoint;

EyePoint myEye;
EyePoint vPoint;
GLfloat vAngle = 0;
/*  多光源	*/
GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light1_position[] = { -2.0, 2.0, 1.0, 1.0 };
GLfloat spot_direction[] = { -1.0, -1.0, 0.0 };
//todo
//hint: some additional parameters may needed here when you operate the teapot
float lightPara[4] = { 1.0,1.0,1.0,0.0 };
float plane[8][3] = { {0,0,1.5},{0,0,1.5},{0,0,1.5},{0,0,1.5},{0,0,1.5},{0, 0, 1.5},{0,0,1.5 },{0,0,0} };
float tRotate = 0.0f;
//*******test

int board[3][3];   //存储几个方块的颜色

#define BUFSIZE 512
int choose = 0;
float mfTranslate;
float mfRotate = 0.0f;
float mfScale = 1.0f;	// set inital scale value to 1.0f

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;
float pos[8][3] = { {480, 285, 390}, {490,290,400},{500,295,410},{510,300,420},{520,305,430}, {530,310,440},{ 530,325,440 },{ 530,325,440 } };

Model model[10];
MS3DModel model2[10];
ObjLoader objloader;
ObjLoader objloader2;
CParticle particle;
//Map map;
int count1 = 0;
int count2 = 0;
//todo
//hint: some additional parameters may needed here when you operate the teapot
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}
void SetupRC()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}
//设置光照  
void SetupLights()
{
	GLfloat ambientLight[] = { 0.2f,0.2f,0.2f,1.0f };//白色主光源环境光  
	GLfloat diffuseLight[] = { 0.8f,0.8f,0.8f,1.0f };//白色主光源漫反射  
	GLfloat specularLight[] = { 1.0f,1.0f,1.0f,1.0f };//白色主光源镜面光  

	GLfloat redAmbiLight[] = { 0.2f,0.0f,0.0f,1.0f };//红色主光源环境光  
	GLfloat redDiffLight[] = { 0.8f,0.0f,0.0f,1.0f };//红色主光源漫反射  
	GLfloat redSpecLight[] = { 1.0f,0.0f,0.0f,1.0f };//红色主光源镜面光  

	GLfloat lightPos[] = { 50.0f,120.0f,120.0f,1.0f };  //光源位置  
	GLfloat spotLightPos[] = { 0.0f,0.0f,200.0f,1.0f }; //射灯位置  
	GLfloat spotDir[] = { 0.0f,0.0f,-1.0f };            //射灯方向  

	GLfloat redLight[] = { 1.0f,0.0f,0.0f,1.0f };       //红色光源  
	GLfloat greenLight[] = { 0.0f,1.0f,0.0f,1.0f };     //绿色光源  
	GLfloat blueLight[] = { 0.0f,0.0f,1.0f,1.0f };      //蓝色光源  


	glEnable(GL_LIGHTING);                          //启用光照  
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);   //设置环境光源  
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);   //设置漫反射光源  
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight); //设置镜面光源  
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);      //设置灯光位置  
	glEnable(GL_LIGHT0);                            //打开白色主光源  

	glLightfv(GL_LIGHT4, GL_AMBIENT, redAmbiLight);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, redDiffLight);
	glLightfv(GL_LIGHT4, GL_SPECULAR, redSpecLight);
	glLightfv(GL_LIGHT4, GL_POSITION, lightPos);

	glLightfv(GL_LIGHT1, GL_SPECULAR, redLight);      //红色射灯  
	glLightfv(GL_LIGHT1, GL_POSITION, spotLightPos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 85.0f);

	glLightfv(GL_LIGHT2, GL_SPECULAR, greenLight);    //绿色射灯  
	glLightfv(GL_LIGHT2, GL_POSITION, spotLightPos);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDir);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 85.0f);

	glLightfv(GL_LIGHT3, GL_SPECULAR, blueLight);     //蓝色射灯  
	glLightfv(GL_LIGHT3, GL_POSITION, spotLightPos);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDir);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 85.0f);


	glEnable(GL_COLOR_MATERIAL);                        //启用材质的颜色跟踪  
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);   //指定材料着色的面  
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight);   //指定材料对镜面光的反应  
	glMateriali(GL_FRONT, GL_SHININESS, 100);             //指定反射系数  
}

				   //处理点击记录:
				   //hits为产生的点击的数量，buffer中存储点击记录，每个点击记录由四个项目组成
void processHits(GLint hits, GLuint buffer[])
{
	unsigned int i, j;
	GLuint ii, jj, names, *ptr;

	ptr = (GLuint *)buffer;



	for (i = 0; i<hits; i++)    //处理每一个点击记录
	{

		//某一个点击记录来说，由四个项目组成：
		//（1）当点击发生时，名字堆栈中的名称数量

		//（2）自上一个点击记录之后，与视景体相交的所有顶点的最小和最大窗口坐标z值
		//（3）当点击发生时，名称堆栈的内容，从最底部的元素开始

		names = *ptr;      //获得名字堆栈中的名称数量
		ptr += 3;               //跳过前三个记录

		for (j = 0; j<names; j++) //开始处理名字堆栈中的内容，获取被选中的方块的index
		{
			//对应于绘制方块时，压入名字堆栈中的内容

			if (j == 0)        //x方向上的index
				ii = *ptr;
			else if (j == 1)  //y方向上的index
				jj = *ptr;
			ptr++;
		}

	}
	board[ii][jj] = (board[ii][jj] + 1) % 3;   //改变被选中方块的颜色

}



//绘制所有方块，参数有GL_RENDER和GL_SELECT两种模式
void drawSquares(GLenum mode)
{
	GLuint i, j;

	for (i = 0; i<3; i++)
	{
		if (mode == GL_SELECT)       //如果是在选择模式下，将名字堆栈的首元素换成x方向上的索引
			glLoadName(i);



		for (j = 0; j<3; j++)
		{
			if (mode == GL_SELECT)    //将y方向上的索引压入名字堆栈
				glPushName(j);



			//绘制方块，在GL_SELECT模式下，某一个方块会被选中，因此，会产生一个点击记录

			//该点击被记录时，名字堆栈中有两个名称，分别是i和j的值，也就是被选中方块的索引
			glColor3f((GLfloat)i / 3.0, (GLfloat)j / 3.0, (GLfloat)board[i][j] / 3.0);
			glRecti(i, j, i + 1, j + 1);



			if (mode == GL_SELECT) //弹出名字
				glPopName();
		}
	}
}



//当鼠标左键点击窗口时，进入选择模式开始绘制；绘制之后，处理点击记录
void pickSquares(int button, int state, int x, int y)
{
	GLuint selectBuf[BUFSIZE];   //存储点击记录
	GLint hits;                                 //点击记录的个数
	GLint viewport[4];                    //视口



	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;



	glGetIntegerv(GL_VIEWPORT, viewport);  //获得视口

	glSelectBuffer(BUFSIZE, selectBuf);    //指定存储点击记录的数组
	glRenderMode(GL_SELECT);          //进入选择模式



	glInitNames();           //初始化名字堆栈并压入初始元素
	glPushName(0);



	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	//设置挑选矩阵，挑选区域的中心坐标是(x,viewport[3]-y)，大小是(5,5)
	gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 5.0, 5.0, viewport);
	//设置投影矩阵
	gluOrtho2D(0.0, 3.0, 0.0, 3.0);

	//在选择模式下绘制方块
	drawSquares(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();        //绘制结束



					  //处理点击记录

	hits = glRenderMode(GL_RENDER); //获取记录下的点击的个数
	processHits(hits, selectBuf);           //处理点击记录selectBuf

	glutPostRedisplay();
}



void initbox()
{
//	glEnable(GL_DEPTH_TEST);
//	glShadeModel(GL_FLAT);
	for (int i = 0; i <3; i++)            //初始化9个方块的颜色
		for (int j = 0; j<3; j++)
			board[i][j] = 0;
}
void init(void)
{
	//m_Shooter = *new Shooter();
	//m_SkyBox = *new CSkyBox();
	/** 用户自定义的初始化过程 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	/** 启用纹理 */
	glEnable(GL_TEXTURE_2D);
	//map.LoadFile("F:/图形学/专题研讨lab/专题研讨lab/testsky/testsky/testsky/resources/objects/map/map.obj",
		//"F:/图形学/专题研讨lab/专题研讨lab/testsky/testsky/testsky/resources/objects/map/map.mtl");
	//objloader.loadFromFile("F:/图形学/专题研讨lab/专题研讨lab/testsky/testsky/testsky/resources/objects/nanosuit/nanosuit.obj",
		//"F:/图形学/专题研讨lab/专题研讨lab/testsky/testsky/testsky/resources/objects/nanosuit/nanosuit.mtl");
	/*
	for (int i = 0; i < 10; i++) {
		model[i] .LoadObj(&objloader);
		model[i].Scale(0.02);
		model[i].Translate(rand()%5, rand() % 5, 0);
	}*/
	objloader2.loadFromFile("F:/图形学/专题研讨lab/专题研讨lab/testsky/testsky/testsky/resources/objects/gun/gun.obj",
		"F:/图形学/专题研讨lab/专题研讨lab/testsky/testsky/testsky/resources/objects/gun/gun.mtl");
	m_Shooter.gun.LoadObj(&objloader2);
	for (int i = 0; i < 10; i++) {
		m_Shooter.gun.Scale(0.02);
		model2[i].loadModelData("F:/图形学/专题研讨lab/专题研讨lab/testsky/testsky/testsky/resources/objects/恐怖行动包/terror_running_with_AK47.MS3D");
		model2[i].setsize(0.01);
		model2[i].Translate(rand() % 5, rand() % 5, 0);
	}
	/** 初始化天空 */
	if (!m_SkyBox.Init())
	{
		//        MessageBox(NULL, (LPCWSTR)"初始化天空失败!", (LPCWSTR)"错误", MB_OK);
		//        exit(0);
		//cout << "初始化天空盒失败" << endl;
	}

	/** 设置摄像机 */
	//m_Shooter.setShooter(500, 300, 400, 502, 300, 400, 0, 1, 0);
	for(int i=0;i<10;i++)
		model2[i].SetupJoints();
	m_Shooter.setCamera(0, 0,5, 0, 0, 1, 0, 1, 0);
}
void Box(float x, float y, float z)
{
	glPushMatrix();//压入堆栈
	glScalef(x, y, z);
	//glEnable(GL_TEXTURE_2D);		//使用纹理
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);// 前
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// 后
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);// 上
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// 下
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);// 左
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// 右
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void airplane(float x, float y, float z, GLfloat r)//组合飞机
{
	glPushMatrix();//压入堆栈
	glTranslatef(x, y, z);//飞机的定位
	glRotatef(-r, 0.0, 1.0, 0.0);//飞机的旋转
	glTranslatef(30, 0, 0);		 //飞机的旋转半径
	glRotatef(30, 0.0, 0.0, 1.0);//飞机的倾斜
								 //=============================================
	glPushMatrix();//
	glRotatef(-r * 30, 0.0, 0.0, 1.0);//飞机的旋转
	glColor3f(0.0, 0.0, 1.0);
	Box(1.0f, 0.1f, 0.02f);		//方，螺旋浆	
	glPopMatrix();
	glColor3f(1.0, 1.0, 1.0);		//白色
									//	glEnable(GL_TEXTURE_2D);		//使用纹理
//	glBindTexture(GL_TEXTURE_2D, g_cactus[1]);//
	glTranslatef(0.0f, 0.0f, -0.5f);	//后移
	gluSphere(g_text, 0.4f, 8, 8);  //机头园(半径)
									//=============================================
	glTranslatef(0.0f, -0.0f, -2);	//位置调整,与机头对接
	gluCylinder(g_text, 0.4, 0.4, 2.0, 8, 4);//机身,园柱(半径、高)
											 //=====================================================
	glRotatef(-180, 1.0, 0.0, 0.0); //角度调整
	glTranslatef(0.0f, -0.0f, 0.0f);	//位置调整,缩进一点		
	gluCylinder(g_text, 0.4, 0.1, 1.5, 8, 4);//机尾,园锥(底半径、高)
											 //======================================================
	//glBindTexture(GL_TEXTURE_2D, g_cactus[0]);//
	glTranslatef(0.0f, -0.8f, 1.2f);	//位置调整
	Box(1.0, 0.05f, 0.3f);			//后翼
	glTranslatef(0.0f, 0.1f, 0.0f);	//位置调整
	Box(0.05f, 0.6f, 0.30f);			//后垂翼
										//======================================================
	glTranslatef(0.0f, 0.7f, -1.9f);	//位置调整
	Box(3, 0.05f, 0.5f);				//前翼
										//======================================================
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void light(float x, float y, float z, float a)//光
{
	GLfloat light_position[] = { x,y,z,a };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
}


void getFPS()
{
	static int frame = 0, time, timebase = 0;
	static char buffer[256];
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(buffer, "FPS:%4.2f X:%.1f,Y%.1f,Z:%.1f",
			frame*1000.0 / (time - timebase),m_Shooter.getPosition().x, m_Shooter.getPosition().y, m_Shooter.getPosition().z);
		timebase = time;
		frame = 0;
	}
	char *c;
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glOrtho(0, 480, 0, 480, -1, 1);    // 位置正投影
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glRasterPos2f(10, 10);
	for (c = buffer; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glEnable(GL_DEPTH_TEST);
}
void display(void)
{
	/** 用户自定义的绘制过程 */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if(count2==0){
		//cout << "update:" <<count1<< endl;
		//model2.update();
		for(int i=0;i<10;i++)
			model2[i].SetFrame(count1);
		//model2.SetFrame(2);
		count1 = count1 % 38;
		count1++;
		
	}
	count2++;
	count2 = count2 % 50;

	switch (choose) {
		case 0:
			light(lightPara[0], lightPara[1], lightPara[2], lightPara[3]);
		break;
		case 1:case 2: case 3: case 4: case 5:
			SetupRC();
			SetupLights();
			break;
	}

		if (roam) {
			if (duration < 600.0f) {
				m_Shooter.goCamera(0.012f);
				duration += 0.05f;
				durationYaw = 0.0005f;
			}
			else if (durationYaw < 100.0f) {
				durationYaw += 0.005f;
				m_Shooter.rotateView(0.0005f, 0, 1, 0);
			}
			else {
				duration = 0.1f;
			}
		}

	/** 放置摄像机 */
	m_Shooter.setLook();
	   
	/** 绘制天空 */
	//m_SkyBox.CreateSkyBox(0, 0, 0, 2, 2, 2);

	//map.Render();
	//绘制飞机
	switch (selectCnt) {
	case 0:case 1:case 2: case 3: case 4: case 5:
		airplane(pos[0][0], pos[0][1], pos[0][2], plane[0][2]);
		airplane(pos[1][0], pos[1][1], pos[1][2], plane[1][2]);
		airplane(pos[2][0], pos[2][1], pos[2][2], plane[2][2]);
		airplane(pos[3][0], pos[3][1], pos[3][2], plane[3][2]);
		airplane(pos[4][0], pos[4][1], pos[4][2], plane[4][2]);
		airplane(pos[5][0], pos[5][1], pos[5][2], plane[5][2]);
		if (plane[selectCnt][2] > 0) plane[selectCnt][2] += 0.05f;
		break;
	case 6://全部旋转
		airplane(pos[0][0], pos[0][1], pos[0][2], plane[selectCnt][2]);
		airplane(pos[1][0], pos[1][1], pos[1][2], plane[selectCnt][2]);
		airplane(pos[2][0], pos[2][1], pos[2][2], plane[selectCnt][2]);
		airplane(pos[3][0], pos[3][1], pos[3][2], plane[selectCnt][2]);
		airplane(pos[4][0], pos[4][1], pos[4][2], plane[selectCnt][2]);
		airplane(pos[5][0], pos[5][1], pos[5][2], plane[selectCnt][2]);
		if (plane[selectCnt][2] > 0) plane[selectCnt][2] += 0.05f;
		break;
	case 7://全部停止
		airplane(pos[0][0], pos[0][1], pos[0][2], plane[selectCnt][2]);
		airplane(pos[1][0], pos[1][1], pos[1][2], plane[selectCnt][2]);
		airplane(pos[2][0], pos[2][1], pos[2][2], plane[selectCnt][2]);
		airplane(pos[3][0], pos[3][1], pos[3][2], plane[selectCnt][2]);
		airplane(pos[4][0], pos[4][1], pos[4][2], plane[selectCnt][2]);
		airplane(pos[5][0], pos[5][1], pos[5][2], plane[selectCnt][2]);
		if (plane[selectCnt][2] > 0) plane[selectCnt][2] += 0.5f;
		break;
	}
	m_Shooter.shot();
	/*
	for (int i = 0; i < 3; i++) {
		model[i].render();
		model[i].DrawModelEdge();
		//model[i].move((model[i].position - m_Shooter.getPosition()), 0.2);
		model[i].setDir((-model[i].position + m_Shooter.getPosition()));
		m_Shooter.is_injured(&model[i]);
		if (m_Shooter.is_hit(&model[i]))
			model[i].explode = true;
	}*/



	m_Shooter.Draw_aim();
	//particle.Draw();
	for(int i=0;i<10;i++)
		model2[i].render();

	m_Shooter.renderGun();

	getFPS();
	glutSwapBuffers();
	glFlush();                   /**< 强制执行所有的OpenGL命令 */

}
//bool LoadT8(char *filename, GLuint &texture)//调8位贴图
//{
//	AUX_RGBImageRec *pImage = NULL;
//	pImage = auxDIBImageLoad(filename);			// 装入位图			
//	if (pImage == NULL)		return false;		// 确保位图数据已经装入
//	glGenTextures(1, &texture);					// 生成纹理
//	glBindTexture(GL_TEXTURE_2D, texture);	// 捆绑纹理
//	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, pImage->sizeX,
//		pImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);
//	free(pImage->data);	//释放位图占据的内存资源
//	free(pImage);
//	return true;// 返回true
//}
void ChangeSize(int width, int height)
{
	glViewport(0, 0, width, height);                                    /**< 重新设置视口 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.01f, 4000000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void motion(int x, int y)
{

	m_Shooter.setViewByMouse(x, y);
	glutPostRedisplay();
}
void click(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN) {
		m_Shooter.addBullet();
	}

}
void keyboard(unsigned char key, int x, int y)
{

    switch (key) {
        case 27:
            exit(0);
            break;
        case 122:
            m_Shooter.setSpeed(2.0f);
            break;
        case 120:
            m_Shooter.setSpeed(4.0f);
            break;
        case 113:
            m_Shooter.moveCamera(m_Shooter.getSpeed()*3);
            break;
        case 101:
            m_Shooter.moveCamera(-m_Shooter.getSpeed()*3);
            break;
        case 97:
	        m_Shooter.yawCamera(-m_Shooter.getSpeed());
            break;
        case 100:
            m_Shooter.yawCamera(m_Shooter.getSpeed());
			break;
		case 119:
			m_Shooter.goCamera(m_Shooter.getSpeed());
            break;
		case 115:
			m_Shooter.goCamera(-m_Shooter.getSpeed());
			break;
		case 102:
			roam = !roam;
			//cout << ">>>>>>>>>>>>>>>>" << endl;
			break;
		case 106://j
			pos[selectCnt][0] -= 1;
			//plane[0] -= 1;
			break;
		case 108://l
			pos[selectCnt][0] += 1;
			//plane[0] += 1;
			break;
		case 107:
			pos[selectCnt][1] -= 1;
			//plane[1] -= 1;
			break;
		case 105://i
			pos[selectCnt][1] += 1;
			//plane[1] += 1;
			break;
		case 111:
			plane[selectCnt][2] = (-1)*plane[selectCnt][2];
			break;
		case 9:
			selectCnt = (selectCnt + 1) % 8;
			break;
		case 52://<-
			lightPara[0] -= 100;
			break;
		case 56://8
			lightPara[1] += 100;
			break;
		case 54://6
			lightPara[0] += 100;
			break;
		case 50://2
			lightPara[1] -= 100;
			break;
		case 49://1
			lightPara[2] += 100;
			break;
		case 51://3
			lightPara[2] -= 100;
			break;
		case 55://a up
			lightPara[3] += 100;
			break;
		case 57://a down
			lightPara[3] -= 100;
			break;
		case 99:
			choose = 1;
			glDisable(GL_LIGHT2);
			glDisable(GL_LIGHT3);
			glEnable(GL_LIGHT1);
			break;
		case 118:
			choose = 2;
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHT3);
			glEnable(GL_LIGHT2);
			break;
		case 98:
			choose = 3;
			glDisable(GL_LIGHT2);
			glDisable(GL_LIGHT1);
			glEnable(GL_LIGHT3);
			break;
		case 110:
			choose = 4;
			glDisable(GL_LIGHT0);
			glEnable(GL_LIGHT4);
			break;
		case 109:
			choose = 5;
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHT2);
			glDisable(GL_LIGHT3);
			glDisable(GL_LIGHT4);
			glEnable(GL_LIGHT0);
			break;
		case 4:
			choose = 0;
			glDisable(GL_LIGHT0);
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) >> 1) - 400, (GetSystemMetrics(SM_CYSCREEN) >> 1) - 300);
	glutCreateWindow("虚拟环境绘制");
	init();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutPassiveMotionFunc(motion);
	glutMouseFunc(click);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
