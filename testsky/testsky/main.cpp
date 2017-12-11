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
/*  ���Դ	*/
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

int board[3][3];   //�洢�����������ɫ

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
//���ù���  
void SetupLights()
{
	GLfloat ambientLight[] = { 0.2f,0.2f,0.2f,1.0f };//��ɫ����Դ������  
	GLfloat diffuseLight[] = { 0.8f,0.8f,0.8f,1.0f };//��ɫ����Դ������  
	GLfloat specularLight[] = { 1.0f,1.0f,1.0f,1.0f };//��ɫ����Դ�����  

	GLfloat redAmbiLight[] = { 0.2f,0.0f,0.0f,1.0f };//��ɫ����Դ������  
	GLfloat redDiffLight[] = { 0.8f,0.0f,0.0f,1.0f };//��ɫ����Դ������  
	GLfloat redSpecLight[] = { 1.0f,0.0f,0.0f,1.0f };//��ɫ����Դ�����  

	GLfloat lightPos[] = { 50.0f,120.0f,120.0f,1.0f };  //��Դλ��  
	GLfloat spotLightPos[] = { 0.0f,0.0f,200.0f,1.0f }; //���λ��  
	GLfloat spotDir[] = { 0.0f,0.0f,-1.0f };            //��Ʒ���  

	GLfloat redLight[] = { 1.0f,0.0f,0.0f,1.0f };       //��ɫ��Դ  
	GLfloat greenLight[] = { 0.0f,1.0f,0.0f,1.0f };     //��ɫ��Դ  
	GLfloat blueLight[] = { 0.0f,0.0f,1.0f,1.0f };      //��ɫ��Դ  


	glEnable(GL_LIGHTING);                          //���ù���  
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);   //���û�����Դ  
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);   //�����������Դ  
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight); //���þ����Դ  
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);      //���õƹ�λ��  
	glEnable(GL_LIGHT0);                            //�򿪰�ɫ����Դ  

	glLightfv(GL_LIGHT4, GL_AMBIENT, redAmbiLight);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, redDiffLight);
	glLightfv(GL_LIGHT4, GL_SPECULAR, redSpecLight);
	glLightfv(GL_LIGHT4, GL_POSITION, lightPos);

	glLightfv(GL_LIGHT1, GL_SPECULAR, redLight);      //��ɫ���  
	glLightfv(GL_LIGHT1, GL_POSITION, spotLightPos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 85.0f);

	glLightfv(GL_LIGHT2, GL_SPECULAR, greenLight);    //��ɫ���  
	glLightfv(GL_LIGHT2, GL_POSITION, spotLightPos);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDir);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 85.0f);

	glLightfv(GL_LIGHT3, GL_SPECULAR, blueLight);     //��ɫ���  
	glLightfv(GL_LIGHT3, GL_POSITION, spotLightPos);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDir);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 85.0f);


	glEnable(GL_COLOR_MATERIAL);                        //���ò��ʵ���ɫ����  
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);   //ָ��������ɫ����  
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight);   //ָ�����϶Ծ����ķ�Ӧ  
	glMateriali(GL_FRONT, GL_SHININESS, 100);             //ָ������ϵ��  
}

				   //��������¼:
				   //hitsΪ�����ĵ����������buffer�д洢�����¼��ÿ�������¼���ĸ���Ŀ���
void processHits(GLint hits, GLuint buffer[])
{
	unsigned int i, j;
	GLuint ii, jj, names, *ptr;

	ptr = (GLuint *)buffer;



	for (i = 0; i<hits; i++)    //����ÿһ�������¼
	{

		//ĳһ�������¼��˵�����ĸ���Ŀ��ɣ�
		//��1�����������ʱ�����ֶ�ջ�е���������

		//��2������һ�������¼֮�����Ӿ����ཻ�����ж������С����󴰿�����zֵ
		//��3�����������ʱ�����ƶ�ջ�����ݣ�����ײ���Ԫ�ؿ�ʼ

		names = *ptr;      //������ֶ�ջ�е���������
		ptr += 3;               //����ǰ������¼

		for (j = 0; j<names; j++) //��ʼ�������ֶ�ջ�е����ݣ���ȡ��ѡ�еķ����index
		{
			//��Ӧ�ڻ��Ʒ���ʱ��ѹ�����ֶ�ջ�е�����

			if (j == 0)        //x�����ϵ�index
				ii = *ptr;
			else if (j == 1)  //y�����ϵ�index
				jj = *ptr;
			ptr++;
		}

	}
	board[ii][jj] = (board[ii][jj] + 1) % 3;   //�ı䱻ѡ�з������ɫ

}



//�������з��飬������GL_RENDER��GL_SELECT����ģʽ
void drawSquares(GLenum mode)
{
	GLuint i, j;

	for (i = 0; i<3; i++)
	{
		if (mode == GL_SELECT)       //�������ѡ��ģʽ�£������ֶ�ջ����Ԫ�ػ���x�����ϵ�����
			glLoadName(i);



		for (j = 0; j<3; j++)
		{
			if (mode == GL_SELECT)    //��y�����ϵ�����ѹ�����ֶ�ջ
				glPushName(j);



			//���Ʒ��飬��GL_SELECTģʽ�£�ĳһ������ᱻѡ�У���ˣ������һ�������¼

			//�õ������¼ʱ�����ֶ�ջ�����������ƣ��ֱ���i��j��ֵ��Ҳ���Ǳ�ѡ�з��������
			glColor3f((GLfloat)i / 3.0, (GLfloat)j / 3.0, (GLfloat)board[i][j] / 3.0);
			glRecti(i, j, i + 1, j + 1);



			if (mode == GL_SELECT) //��������
				glPopName();
		}
	}
}



//���������������ʱ������ѡ��ģʽ��ʼ���ƣ�����֮�󣬴�������¼
void pickSquares(int button, int state, int x, int y)
{
	GLuint selectBuf[BUFSIZE];   //�洢�����¼
	GLint hits;                                 //�����¼�ĸ���
	GLint viewport[4];                    //�ӿ�



	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;



	glGetIntegerv(GL_VIEWPORT, viewport);  //����ӿ�

	glSelectBuffer(BUFSIZE, selectBuf);    //ָ���洢�����¼������
	glRenderMode(GL_SELECT);          //����ѡ��ģʽ



	glInitNames();           //��ʼ�����ֶ�ջ��ѹ���ʼԪ��
	glPushName(0);



	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	//������ѡ������ѡ���������������(x,viewport[3]-y)����С��(5,5)
	gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 5.0, 5.0, viewport);
	//����ͶӰ����
	gluOrtho2D(0.0, 3.0, 0.0, 3.0);

	//��ѡ��ģʽ�»��Ʒ���
	drawSquares(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();        //���ƽ���



					  //��������¼

	hits = glRenderMode(GL_RENDER); //��ȡ��¼�µĵ���ĸ���
	processHits(hits, selectBuf);           //��������¼selectBuf

	glutPostRedisplay();
}



void initbox()
{
//	glEnable(GL_DEPTH_TEST);
//	glShadeModel(GL_FLAT);
	for (int i = 0; i <3; i++)            //��ʼ��9���������ɫ
		for (int j = 0; j<3; j++)
			board[i][j] = 0;
}
void init(void)
{
	//m_Shooter = *new Shooter();
	//m_SkyBox = *new CSkyBox();
	/** �û��Զ���ĳ�ʼ������ */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	/** �������� */
	glEnable(GL_TEXTURE_2D);
	//map.LoadFile("F:/ͼ��ѧ/ר������lab/ר������lab/testsky/testsky/testsky/resources/objects/map/map.obj",
		//"F:/ͼ��ѧ/ר������lab/ר������lab/testsky/testsky/testsky/resources/objects/map/map.mtl");
	//objloader.loadFromFile("F:/ͼ��ѧ/ר������lab/ר������lab/testsky/testsky/testsky/resources/objects/nanosuit/nanosuit.obj",
		//"F:/ͼ��ѧ/ר������lab/ר������lab/testsky/testsky/testsky/resources/objects/nanosuit/nanosuit.mtl");
	/*
	for (int i = 0; i < 10; i++) {
		model[i] .LoadObj(&objloader);
		model[i].Scale(0.02);
		model[i].Translate(rand()%5, rand() % 5, 0);
	}*/
	objloader2.loadFromFile("F:/ͼ��ѧ/ר������lab/ר������lab/testsky/testsky/testsky/resources/objects/gun/gun.obj",
		"F:/ͼ��ѧ/ר������lab/ר������lab/testsky/testsky/testsky/resources/objects/gun/gun.mtl");
	m_Shooter.gun.LoadObj(&objloader2);
	for (int i = 0; i < 10; i++) {
		m_Shooter.gun.Scale(0.02);
		model2[i].loadModelData("F:/ͼ��ѧ/ר������lab/ר������lab/testsky/testsky/testsky/resources/objects/�ֲ��ж���/terror_running_with_AK47.MS3D");
		model2[i].setsize(0.01);
		model2[i].Translate(rand() % 5, rand() % 5, 0);
	}
	/** ��ʼ����� */
	if (!m_SkyBox.Init())
	{
		//        MessageBox(NULL, (LPCWSTR)"��ʼ�����ʧ��!", (LPCWSTR)"����", MB_OK);
		//        exit(0);
		//cout << "��ʼ����պ�ʧ��" << endl;
	}

	/** ��������� */
	//m_Shooter.setShooter(500, 300, 400, 502, 300, 400, 0, 1, 0);
	for(int i=0;i<10;i++)
		model2[i].SetupJoints();
	m_Shooter.setCamera(0, 0,5, 0, 0, 1, 0, 1, 0);
}
void Box(float x, float y, float z)
{
	glPushMatrix();//ѹ���ջ
	glScalef(x, y, z);
	//glEnable(GL_TEXTURE_2D);		//ʹ������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);// ǰ
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// ��
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);// ��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// ��
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);// ��
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// ��
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void airplane(float x, float y, float z, GLfloat r)//��Ϸɻ�
{
	glPushMatrix();//ѹ���ջ
	glTranslatef(x, y, z);//�ɻ��Ķ�λ
	glRotatef(-r, 0.0, 1.0, 0.0);//�ɻ�����ת
	glTranslatef(30, 0, 0);		 //�ɻ�����ת�뾶
	glRotatef(30, 0.0, 0.0, 1.0);//�ɻ�����б
								 //=============================================
	glPushMatrix();//
	glRotatef(-r * 30, 0.0, 0.0, 1.0);//�ɻ�����ת
	glColor3f(0.0, 0.0, 1.0);
	Box(1.0f, 0.1f, 0.02f);		//����������	
	glPopMatrix();
	glColor3f(1.0, 1.0, 1.0);		//��ɫ
									//	glEnable(GL_TEXTURE_2D);		//ʹ������
//	glBindTexture(GL_TEXTURE_2D, g_cactus[1]);//
	glTranslatef(0.0f, 0.0f, -0.5f);	//����
	gluSphere(g_text, 0.4f, 8, 8);  //��ͷ԰(�뾶)
									//=============================================
	glTranslatef(0.0f, -0.0f, -2);	//λ�õ���,���ͷ�Խ�
	gluCylinder(g_text, 0.4, 0.4, 2.0, 8, 4);//����,԰��(�뾶����)
											 //=====================================================
	glRotatef(-180, 1.0, 0.0, 0.0); //�Ƕȵ���
	glTranslatef(0.0f, -0.0f, 0.0f);	//λ�õ���,����һ��		
	gluCylinder(g_text, 0.4, 0.1, 1.5, 8, 4);//��β,԰׶(�װ뾶����)
											 //======================================================
	//glBindTexture(GL_TEXTURE_2D, g_cactus[0]);//
	glTranslatef(0.0f, -0.8f, 1.2f);	//λ�õ���
	Box(1.0, 0.05f, 0.3f);			//����
	glTranslatef(0.0f, 0.1f, 0.0f);	//λ�õ���
	Box(0.05f, 0.6f, 0.30f);			//����
										//======================================================
	glTranslatef(0.0f, 0.7f, -1.9f);	//λ�õ���
	Box(3, 0.05f, 0.5f);				//ǰ��
										//======================================================
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void light(float x, float y, float z, float a)//��
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
	glMatrixMode(GL_PROJECTION);  // ѡ��ͶӰ����
	glPushMatrix();               // ����ԭ����
	glLoadIdentity();             // װ�뵥λ����
	glOrtho(0, 480, 0, 480, -1, 1);    // λ����ͶӰ
	glMatrixMode(GL_MODELVIEW);   // ѡ��Modelview����
	glPushMatrix();               // ����ԭ����
	glLoadIdentity();             // װ�뵥λ����
	glRasterPos2f(10, 10);
	for (c = buffer; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glMatrixMode(GL_PROJECTION);  // ѡ��ͶӰ����
	glPopMatrix();                // ����Ϊԭ�������
	glMatrixMode(GL_MODELVIEW);   // ѡ��Modelview����
	glPopMatrix();                // ����Ϊԭ�������
	glEnable(GL_DEPTH_TEST);
}
void display(void)
{
	/** �û��Զ���Ļ��ƹ��� */
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

	/** ��������� */
	m_Shooter.setLook();
	   
	/** ������� */
	//m_SkyBox.CreateSkyBox(0, 0, 0, 2, 2, 2);

	//map.Render();
	//���Ʒɻ�
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
	case 6://ȫ����ת
		airplane(pos[0][0], pos[0][1], pos[0][2], plane[selectCnt][2]);
		airplane(pos[1][0], pos[1][1], pos[1][2], plane[selectCnt][2]);
		airplane(pos[2][0], pos[2][1], pos[2][2], plane[selectCnt][2]);
		airplane(pos[3][0], pos[3][1], pos[3][2], plane[selectCnt][2]);
		airplane(pos[4][0], pos[4][1], pos[4][2], plane[selectCnt][2]);
		airplane(pos[5][0], pos[5][1], pos[5][2], plane[selectCnt][2]);
		if (plane[selectCnt][2] > 0) plane[selectCnt][2] += 0.05f;
		break;
	case 7://ȫ��ֹͣ
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
	glFlush();                   /**< ǿ��ִ�����е�OpenGL���� */

}
//bool LoadT8(char *filename, GLuint &texture)//��8λ��ͼ
//{
//	AUX_RGBImageRec *pImage = NULL;
//	pImage = auxDIBImageLoad(filename);			// װ��λͼ			
//	if (pImage == NULL)		return false;		// ȷ��λͼ�����Ѿ�װ��
//	glGenTextures(1, &texture);					// ��������
//	glBindTexture(GL_TEXTURE_2D, texture);	// ��������
//	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, pImage->sizeX,
//		pImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);
//	free(pImage->data);	//�ͷ�λͼռ�ݵ��ڴ���Դ
//	free(pImage);
//	return true;// ����true
//}
void ChangeSize(int width, int height)
{
	glViewport(0, 0, width, height);                                    /**< ���������ӿ� */
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
	glutCreateWindow("���⻷������");
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
