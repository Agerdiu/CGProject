#include "Particle.h"
#include "reference.h"

/** ���캯�� */
CParticle::CParticle()
{
}

/** �������� */
CParticle::~CParticle()
{
	cout << "Particle has been free" << endl;
	data.clear();
}

/** ����һ������num��Ԫ�ص��������� */
int CParticle::Create(long num)
{
	data.clear();
	for (int i = 0; i < num; i++) {
		data.push_back(Particle());
	}
	return 1;
}


/** ���úͻ�ȡ��ɫColor�ĺ���ʵ�� */
int CParticle::SetColor(GLint r, GLint g, GLint b)
{
	for (int index = 0; index<data.size(); ++index)
	{
		data[index].r = r;
		data[index].g = g;
		data[index].b = b;
	}
	return true;
}

int CParticle::SetColor(GLint index, GLint r, GLint g, GLint b)
{
	if (index >= 0 && index<data.size())
	{
		data[index].r = r;
		data[index].g = g;
		data[index].b = b;
		return true;
	}
	return false;
}

int CParticle::GetColor(GLint index, GLint &r, GLint &g, GLint &b)
{
	if (index >= 0 && index<data.size())
	{
		r = data[index].r;
		g = data[index].g;
		b = data[index].b;
		return true;
	}
	return false;
}

/** ���úͻ�ȡλ��Position�ĺ���ʵ�� */
int CParticle::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	for (int index = 0; index<data.size(); ++index)
	{
		data[index].x = x;
		data[index].y = y;
		data[index].z = z;
	}
	return true;
}

int CParticle::SetPosition(GLint index, GLfloat x, GLfloat y, GLfloat z)
{
	if (index >= 0 && index<data.size())
	{
		data[index].x = x;
		data[index].y = y;
		data[index].z = z;
		return true;
	}
	return false;
}

int CParticle::GetPosition(GLint index, GLfloat &x, GLfloat &y, GLfloat &z)
{
	if (index >= 0 && index<data.size())
	{
		x = data[index].x;
		y = data[index].y;
		z = data[index].z;
		return true;
	}
	return false;
}
/** ���úͻ�ȡ���ٶ�Acceleration�ĺ���ʵ�� */
int CParticle::SetAcceleration(GLfloat ax, GLfloat ay, GLfloat az)
{
	for (int index = 0; index<data.size(); ++index)
	{
		data[index].ax = ax;
		data[index].ay = ay;
		data[index].az = az;
	}
	return true;
}

int CParticle::SetAcceleration(GLint index, GLfloat ax, GLfloat ay, GLfloat az)
{
	if (index >= 0 && index<data.size())
	{
		data[index].ax = ax;
		data[index].ay = ay;
		data[index].az = az;
		return true;
	}
	return false;
}


int CParticle::GetAcceletation(GLint index, GLfloat &ax, GLfloat &ay, GLfloat &az)
{
	if (index >= 0 && index<data.size())
	{
		ax = data[index].ax;
		ay = data[index].ay;
		az = data[index].az;
		return true;
	}
	return false;
}



/** Velocity������ʵ�� */
int CParticle::SetVelocity(GLfloat vx, GLfloat vy, GLfloat vz)
{
	for (int index = 0; index<data.size(); ++index)
	{
		data[index].vx = vx;
		data[index].vy = vy;
		data[index].vz = vz;
	}
	return true;
}

int CParticle::SetVelocity(GLint index, GLfloat vx, GLfloat vy, GLfloat vz)
{
	if (index >= 0 && index<data.size())
	{
		data[index].vx = vx;
		data[index].vy = vy;
		data[index].vz = vz;
		return true;
	}
	return false;
}

int CParticle::GetVelocity(GLint index, GLfloat &vx, GLfloat &vy, GLfloat &vz)
{
	if (index >= 0 && index<data.size())
	{
		vx = data[index].vx;
		vy = data[index].vy;
		vz = data[index].vz;
		return true;
	}
	return false;
}


/** Size������ʵ�� */
int CParticle::SetSize(GLfloat size)
{
	for (int index = 0; index<data.size(); ++index)
	{
		data[index].size = size;
	}
	return true;
}

int CParticle::SetSize(GLint index, GLfloat size)
{
	if (index >= 0 && index<data.size())
	{
		data[index].size = size;
		return true;
	}
	return false;
}

int CParticle::GetSize(GLint index, GLfloat &size)
{
	if (index >= 0 && index < data.size())
	{
		size = data[index].size;
		return true;
	}
	return false;
}

/** ��ʧ�ٶ�Dec���� */
int CParticle::SetDec(GLfloat dec)
{
	for (int index = 0; index<data.size(); ++index)
	{
		data[index].dec = dec;
	}
	return true;
}

int CParticle::SetDec(GLint index, GLfloat dec)
{
	if (index >= 0 && index < data.size())
	{
		data[index].dec = dec;
		return true;
	}
	return false;
}

int CParticle::GetDec(GLint index, GLfloat &dec)
{
	if (index >= 0 && index < data.size())
	{
		dec = data[index].dec;
		return true;
	}
	return false;
}

/** �������ӵ�lifetime ���� */
int CParticle::SetLifeTime(GLfloat lifetime)
{
	for (int index = 0; index<data.size(); ++index)
	{
		data[index].lifetime = lifetime;
	}
	return true;
}

int CParticle::SetLifeTime(GLint index, GLfloat lifetime)
{
	if (index >= 0 && index < data.size())
	{
		data[index].lifetime = lifetime;
		return true;
	}
	return false;
}

/** ������ӵ�lifetime���� */
int CParticle::GetLifeTime(GLint index, GLfloat &lifetime)
{
	if (index >= 0 && index < data.size())
	{
		lifetime = data[index].lifetime;
		return true;
	}
	return false;
}

/** ��ȡ���ӵ��������� */
int CParticle::GetAll(int index, GLint &r, GLint &g, GLint &b,         /**< ���ӵ���ɫ */
	GLfloat &x, GLfloat &y, GLfloat &z,     /**< λ�� */
	GLfloat &vx, GLfloat &vy, GLfloat &vz,  /**< �ٶ� */
	GLfloat &ax, GLfloat &ay, GLfloat &az,  /**< ���ٶ� */
	GLfloat &size,                            /**< ��С */
	GLfloat &lifetime,                        /**< ����ʱ�� */
	GLfloat &dec                          /**< ��ʧ�ٶ� */
)
{
	if (index >= 0 && index<data.size())
	{
		r = data[index].r;
		g = data[index].g;
		b = data[index].b;
		x = data[index].x;
		y = data[index].y;
		z = data[index].z;
		vx = data[index].vx;
		vy = data[index].vy;
		vz = data[index].vz;
		ax = data[index].ax;
		ay = data[index].ay;
		az = data[index].az;
		lifetime = data[index].lifetime;
		size = data[index].size;
		dec = data[index].dec;
		return true;
	}
	return false;
}

/** �������ӵ��������� */
int CParticle::SetAll(int index, GLint r, GLint g, GLint b,        /**< ���ӵ���ɫ */
	GLfloat x, GLfloat y, GLfloat z,        /**< λ�� */
	GLfloat vx, GLfloat vy, GLfloat vz, /**< �ٶ� */
	GLfloat ax, GLfloat ay, GLfloat az, /**< ���ٶ� */
	GLfloat size,                     /**< ��С */
	GLfloat lifetime,                 /**< ����ʱ�� */
	GLfloat dec                           /**< ��ʧ�ٶ� */
)
{
	if (index >= 0 && index<data.size())
	{
		data[index].r = r;
		data[index].g = g;
		data[index].b = b;
		data[index].x = x;
		data[index].y = y;
		data[index].z = z;
		data[index].vx = vx;
		data[index].vy = vy;
		data[index].vz = vz;
		data[index].ax = ax;
		data[index].ay = ay;
		data[index].az = az;
		data[index].lifetime = lifetime;
		data[index].size = size;
		data[index].dec = dec;
		return true;
	}
	return false;
}
bool CParticle::LoadTextures(char*filename)
{
	return Texture::loadTexture(filename, &loader);
}
bool CParticle::Init()
{

	Create(200);
	double x1 = 0.5f * (rand() % 9);
	double y1 = 0.5f*(rand() % 9);
	double z1 = 0.5f*(rand() % 9);
	for (int i = 0; i < data.size(); ++i)
	{
		///��ʼ����ɫ
		//r = rand()%255;
		//g = rand()%255;
		//b = rand()%255;
		data[i].r = 50;
		data[i].g = 50;
		data[i].b = 100;


		///��ʼ������
		data[i].x = x1;
		data[i].y = y1;
		data[i].z = z1;

		///��ʼ���ٶ�
		data[i].vx = 0.000035f * (rand() % 9-4.5);
		data[i].vy = 0.000035f * (rand() % 9 - 2);
		data[i].vz = 0.000035f * (rand() % 9 - 4.5);


		///��ʼ�����ٶ�
		data[i].ax = 0;
		data[i].ay = -0.0000002f;
		data[i].az = 0;

		///��ʼ����������
		data[i].lifetime = 100000;

		///��ʧ�ٶ�
		data[i].dec = 0.05*(rand() % 50);

		///��ʼ����С
		data[i].size = 0.008f;
	}
	return true;
}
void CParticle::Update(int i)
{
	/** ����λ�� */
	data[i].x += data[i].vx;
	data[i].y += data[i].vy;
	data[i].z += data[i].vz;

	/** �����ٶ� */
	data[i].vy += data[i].ay;
	data[i].vx += data[i].ax;
	data[i].vz += data[i].az;

	/** ��������ʱ�� */
	data[i].lifetime -= data[i].dec;

}
void CParticle::Draw()
{
	float x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, dec;
	int r, g, b;

	//glDepthFunc(GL_LEQUAL);//ָ��������Ȼ���Ƚ�ֵ(���½��������ֵ��ԭ����1�Ƚϣ�<=��ͨ����������)
	//glEnable(GL_DEPTH_TEST);
	//glShadeModel(GL_SMOOTH);

	glEnable(GL_TEXTURE_2D);             /**< ��������ӳ�� */
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);    /**< ���û�����ӻ�ð�͸��Ч�� */
	//glEnable(GL_BLEND);                  /**< ���û�� */
	glBindTexture(GL_TEXTURE_2D, loader.ID);
	vector<Particle>::iterator iter;
	int tmp=0;
	for (iter = data.begin(); iter!=data.end();)
	{

		/** ������ӵ��������� */
		GetAll(tmp, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, dec);
		//cout << x << " " << y << " " << z << endl;
		glPushMatrix();
		//glTranslatef(0.0f, 0.0f, -6.0f);
		//glColor4ub(r, g, b, 255);
		//glNormal3f(0.0f, 0.0f, 1.0f);   /**< ���巨�߷��� */
										/** �������� */
		//glBegin(GL_QUADS);
		GLUquadricObj *g_text = gluNewQuadric();
		gluQuadricTexture(g_text,loader.ID);
		glTranslatef(x, y,z);
		gluSphere(g_text, size, 15, 8);
		//glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size, y - size, z);
		//glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size, y + size, z);
		//glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size, y + size, z);
		//glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size, y - size, z);
		glEnd();
		glPopMatrix();
		/** ������������ */
		if ((*iter).dec < 0 || (*iter).y < -1)
			iter=data.erase(iter);
		else {
			iter++;
			Update(tmp++);
		}
	}

	//glDisable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
}
