#include "AABBbox.h"
#include "reference.h"
#include "glut\glut.h"
//Ĭ�ϵĹ��캯��
AABBbox::AABBbox(void)
{
}
//��AABB��Χ����С���������My3DPoint�ṹ����Ϊ�����Ĺ��캯��
AABBbox::AABBbox(Point3 pointMin, Point3 pointMax)
{
	Xmin = pointMin.x;
	Ymin = pointMin.y;
	Zmin = pointMin.z;
	Xmax = pointMax.x;
	Ymax = pointMax.y;
	Zmax = pointMax.z;
}
//��AABB��Χ����С�������������������Ϊ�����Ĺ��캯��
AABBbox::AABBbox(double m_Xmin, double m_Ymin, double m_Zmin, double m_Xmax, double m_Ymax, double m_Zmax)
{
	Xmin = m_Xmin;
	Xmax = m_Xmax;
	Ymin = m_Ymin;
	Ymax = m_Ymax;
	Zmin = m_Zmin;
	Zmax = m_Zmax;
}
//��Triangle�����νṹ����Ϊ�����Ĺ��캯��
AABBbox::AABBbox(Triangle a)
{
	//����Ԫ���������������Сֵ
	//��X����Сֵ
	Xmin = a.x.x<a.y.x ? a.x.x : a.y.x;
	Xmin = Xmin<a.z.x ? Xmin : a.z.x;
	//��Y����Сֵ
	Ymin = a.x.y<a.y.y ? a.x.y : a.y.y;
	Ymin = Ymin<a.z.y ? Ymin : a.z.y;
	//��Z����Сֵ
	Zmin = a.x.z<a.y.z ? a.x.z : a.y.z;
	Zmin = Zmin<a.z.z ? Zmin : a.z.z;
	//��X�����ֵ
	Xmax = a.x.x>a.y.x ? a.x.x : a.y.x;
	Xmax = Xmax>a.z.x ? Xmax : a.z.x;
	//��Y�����ֵ
	Ymax = a.x.y>a.y.y ? a.x.y : a.y.y;
	Ymax = Ymax>a.z.y ? Ymax : a.z.y;
	//��Z�����ֵ
	Zmax = a.x.z>a.y.z ? a.x.z : a.y.z;
	Zmax = Zmax>a.z.z ? Zmax : a.z.z;
}

/*-----IsOrNotInterection(AABB *a,AABB *b)---------
�������ܣ��ж�����AABB��Χ���Ƿ��ཻ
����������aΪAABB����ָ��,�������Ϊb��Χ��
--------------------------------------------------*/
//�ж�����AABB��Χ���Ƿ��ཻ
bool AABBbox::IsOrNotInterection(AABBbox *a)
{
	if (a->Xmin>Xmax)//���a��Χ����С��X�������b��Χ�е����X����,a��Χ����X��������b���ұ�
	{
		return FALSE;
	}
	else if (a->Xmax<Xmin)//���a��Χ������X����С��b��Χ�е���С��X���꣬a��Χ����X��������b�����
	{
		return FALSE;
	}
	else if (a->Ymin>Ymax)//���a��Χ����С��Y�������b��Χ�е����Y���꣬a��Χ����Y��������b���ұ�
	{
		return FALSE;
	}
	else if (a->Ymax<Ymin)//���a��Χ������Y����С��b��Χ�е���С��Y����,a��Χ����Y��������b�����
	{
		return FALSE;
	}
	else if (a->Zmin>Zmax)//���a��Χ����С��Z�������b��Χ�е����Z���꣬a��Χ����Z��������b���Ϸ�
	{
		return FALSE;
	}
	else if (a->Zmax<Zmin)//���a��Χ������Z����С��b��Χ�е���С��Z����,a��Χ����Z��������b���·�
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}

}
/*-----PointIsOrNotInAABBBoundingBox(My3DPoint pt,AABB *box)�жϵ��Ƿ�����AABB��Χ��֮��------
�������ܣ��ж���ά���Ƿ�����AABB��Χ��֮��
����������ptΪMy3DPoint�ṹ�壬boxΪAABB��Χ�ж���ָ��
---------------------------------------------------------------------------------------------*/

bool AABBbox::PointIsOrNotInAABBBoundingBox(Point3 pt)
{
	return (pt.x >= Xmin) && (pt.x <= Xmax) &&
		(pt.y >= Ymin) && (pt.y <= Ymax) &&
		(pt.z >= Zmin) && (pt.z <= Zmax);
}

/*-----GetAABBBoxCeter()-------------------------------------------
�������ܣ��õ���AABB��Χ�ж�������ģ����ģ�����һ��My3DPoint�ṹ��
-------------------------------------------------------------------*/
Point3 AABBbox::GetAABBBoxCeter()
{
	Point3 m_point;
	m_point.x = (Xmin + Xmax)*0.5;
	m_point.y = (Ymin + Ymax)*0.5;
	m_point.z = (Zmin + Zmax)*0.5;
	return m_point;
}

/*-----IsOrNotEmpty()---------------------------------------------
�������ܣ��ж�AABB��Χ���Ƿ�Ϊ�գ�Ϊ�շ���TRUE����Ϊ�շ���FALSE
------------------------------------------------------------------*/
bool AABBbox::IsOrNotEmpty()
{
	return(Xmin>Xmax) || (Ymin>Ymax) || (Zmin>Zmax);
}
/*-----GetXLength()---------
�������ܣ��õ�X�����ϵĳ���
--------------------------*/
double AABBbox::GetXLength()
{
	return Xmax - Xmin;
}
/*-----GetYLength()---------
�������ܣ��õ�Y�����ϵĳ���
--------------------------*/
double AABBbox::GetYLength()
{
	return Ymax - Ymin;
}
/*-----GetZLength()---------
�������ܣ��õ�Z�����ϵĳ���
--------------------------*/
double AABBbox::GetZLength()
{
	return Zmax - Zmin;
}
/*-----GetVolume()-------------
�������ܣ��õ�AABB��Χ�е����
------------------------------*/
double AABBbox::GetVolume()
{
	return (Xmax - Xmin)*(Ymax - Ymin)*(Zmax - Zmin);
}

/*-----DrawAABBBoundingBox()-------------------
�������ܣ�����AABB��Χ��
-----------------------------------------------*/
void AABBbox::DrawAABBBoundingBox()
{
	//��һ����
	glLineWidth(2);
	glColor3i(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(Xmin, Ymin, Zmin);
	glVertex3f(Xmax, Ymin, Zmin);
	//�ڶ�����
	glVertex3f(Xmin, Ymin, Zmin);
	glVertex3f(Xmin, Ymin, Zmax);
	//��������
	glVertex3f(Xmin, Ymin, Zmin);
	glVertex3f(Xmin, Ymax, Zmin);
	//���ĸ���
	glVertex3f(Xmin, Ymin, Zmax);
	glVertex3f(Xmax, Ymin, Zmax);
	//�������
	glVertex3f(Xmax, Ymin, Zmin);
	glVertex3f(Xmax, Ymin, Zmax);
	//��������
	glVertex3f(Xmax, Ymin, Zmin);
	glVertex3f(Xmax, Ymax, Zmin);
	//���߸���
	glVertex3f(Xmax, Ymax, Zmin);
	glVertex3f(Xmin, Ymax, Zmin);
	//�ڰ˸���
	glVertex3f(Xmin, Ymax, Zmin);
	glVertex3f(Xmin, Ymax, Zmax);
	//�ھŸ���
	glVertex3f(Xmax, Ymax, Zmin);
	glVertex3f(Xmax, Ymax, Zmax);
	//��ʮ����
	glVertex3f(Xmin, Ymin, Zmax);
	glVertex3f(Xmin, Ymax, Zmax);
	//��ʮһ����
	glVertex3f(Xmax, Ymin, Zmax);
	glVertex3f(Xmax, Ymax, Zmax);
	//��ʮ������
	glVertex3f(Xmin, Ymax, Zmax);
	glVertex3f(Xmax, Ymax, Zmax);
	glEnd();
}
AABBbox::~AABBbox(void)
{
}
