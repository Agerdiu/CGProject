#include "AABBbox.h"
#include "reference.h"
#include "glut\glut.h"
//默认的构造函数
AABBbox::AABBbox(void)
{
}
//以AABB包围盒最小最大坐标点的My3DPoint结构体作为参数的构造函数
AABBbox::AABBbox(Point3 pointMin, Point3 pointMax)
{
	Xmin = pointMin.x;
	Ymin = pointMin.y;
	Zmin = pointMin.z;
	Xmax = pointMax.x;
	Ymax = pointMax.y;
	Zmax = pointMax.z;
}
//以AABB包围盒最小最大坐标点的六个坐标作为参数的构造函数
AABBbox::AABBbox(double m_Xmin, double m_Ymin, double m_Zmin, double m_Xmax, double m_Ymax, double m_Zmax)
{
	Xmin = m_Xmin;
	Xmax = m_Xmax;
	Ymin = m_Ymin;
	Ymax = m_Ymax;
	Zmin = m_Zmin;
	Zmax = m_Zmax;
}
//以Triangle三角形结构体作为参数的构造函数
AABBbox::AABBbox(Triangle a)
{
	//用三元运算符来求出最大最小值
	//求X的最小值
	Xmin = a.x.x<a.y.x ? a.x.x : a.y.x;
	Xmin = Xmin<a.z.x ? Xmin : a.z.x;
	//求Y的最小值
	Ymin = a.x.y<a.y.y ? a.x.y : a.y.y;
	Ymin = Ymin<a.z.y ? Ymin : a.z.y;
	//求Z的最小值
	Zmin = a.x.z<a.y.z ? a.x.z : a.y.z;
	Zmin = Zmin<a.z.z ? Zmin : a.z.z;
	//求X的最大值
	Xmax = a.x.x>a.y.x ? a.x.x : a.y.x;
	Xmax = Xmax>a.z.x ? Xmax : a.z.x;
	//求Y的最大值
	Ymax = a.x.y>a.y.y ? a.x.y : a.y.y;
	Ymax = Ymax>a.z.y ? Ymax : a.z.y;
	//求Z的最大值
	Zmax = a.x.z>a.y.z ? a.x.z : a.y.z;
	Zmax = Zmax>a.z.z ? Zmax : a.z.z;
}

/*-----IsOrNotInterection(AABB *a,AABB *b)---------
函数功能：判断两个AABB包围盒是否相交
函数参数：a为AABB对象指针,本身对象为b包围盒
--------------------------------------------------*/
//判断两个AABB包围盒是否相交
bool AABBbox::IsOrNotInterection(AABBbox *a)
{
	if (a->Xmin>Xmax)//如果a包围盒最小的X坐标大于b包围盒的最大X坐标,a包围盒在X方向上在b的右边
	{
		return FALSE;
	}
	else if (a->Xmax<Xmin)//如果a包围盒最大的X坐标小于b包围盒的最小的X坐标，a包围盒在X方向上在b的左边
	{
		return FALSE;
	}
	else if (a->Ymin>Ymax)//如果a包围盒最小的Y坐标大于b包围盒的最大Y坐标，a包围盒在Y方向上在b的右边
	{
		return FALSE;
	}
	else if (a->Ymax<Ymin)//如果a包围盒最大的Y坐标小于b包围盒的最小的Y坐标,a包围盒在Y方向上在b的左边
	{
		return FALSE;
	}
	else if (a->Zmin>Zmax)//如果a包围盒最小的Z坐标大于b包围盒的最大Z坐标，a包围盒在Z方向上在b的上方
	{
		return FALSE;
	}
	else if (a->Zmax<Zmin)//如果a包围盒最大的Z坐标小于b包围盒的最小的Z坐标,a包围盒在Z方向上在b的下方
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}

}
/*-----PointIsOrNotInAABBBoundingBox(My3DPoint pt,AABB *box)判断点是否落在AABB包围盒之内------
函数功能：判断三维点是否落在AABB包围盒之内
函数参数：pt为My3DPoint结构体，box为AABB包围盒对象指针
---------------------------------------------------------------------------------------------*/

bool AABBbox::PointIsOrNotInAABBBoundingBox(Point3 pt)
{
	return (pt.x >= Xmin) && (pt.x <= Xmax) &&
		(pt.y >= Ymin) && (pt.y <= Ymax) &&
		(pt.z >= Zmin) && (pt.z <= Zmax);
}

/*-----GetAABBBoxCeter()-------------------------------------------
函数功能：得到此AABB包围盒对象的质心，中心，返回一个My3DPoint结构体
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
函数功能：判断AABB包围盒是否为空，为空返回TRUE，不为空返回FALSE
------------------------------------------------------------------*/
bool AABBbox::IsOrNotEmpty()
{
	return(Xmin>Xmax) || (Ymin>Ymax) || (Zmin>Zmax);
}
/*-----GetXLength()---------
函数功能：得到X方向上的长度
--------------------------*/
double AABBbox::GetXLength()
{
	return Xmax - Xmin;
}
/*-----GetYLength()---------
函数功能：得到Y方向上的长度
--------------------------*/
double AABBbox::GetYLength()
{
	return Ymax - Ymin;
}
/*-----GetZLength()---------
函数功能：得到Z方向上的长度
--------------------------*/
double AABBbox::GetZLength()
{
	return Zmax - Zmin;
}
/*-----GetVolume()-------------
函数功能：得到AABB包围盒的体积
------------------------------*/
double AABBbox::GetVolume()
{
	return (Xmax - Xmin)*(Ymax - Ymin)*(Zmax - Zmin);
}

/*-----DrawAABBBoundingBox()-------------------
函数功能：绘制AABB包围盒
-----------------------------------------------*/
void AABBbox::DrawAABBBoundingBox()
{
	//第一根线
	glLineWidth(2);
	glColor3i(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(Xmin, Ymin, Zmin);
	glVertex3f(Xmax, Ymin, Zmin);
	//第二根线
	glVertex3f(Xmin, Ymin, Zmin);
	glVertex3f(Xmin, Ymin, Zmax);
	//第三根线
	glVertex3f(Xmin, Ymin, Zmin);
	glVertex3f(Xmin, Ymax, Zmin);
	//第四根线
	glVertex3f(Xmin, Ymin, Zmax);
	glVertex3f(Xmax, Ymin, Zmax);
	//第五根线
	glVertex3f(Xmax, Ymin, Zmin);
	glVertex3f(Xmax, Ymin, Zmax);
	//第六根线
	glVertex3f(Xmax, Ymin, Zmin);
	glVertex3f(Xmax, Ymax, Zmin);
	//第七根线
	glVertex3f(Xmax, Ymax, Zmin);
	glVertex3f(Xmin, Ymax, Zmin);
	//第八根线
	glVertex3f(Xmin, Ymax, Zmin);
	glVertex3f(Xmin, Ymax, Zmax);
	//第九根线
	glVertex3f(Xmax, Ymax, Zmin);
	glVertex3f(Xmax, Ymax, Zmax);
	//第十根线
	glVertex3f(Xmin, Ymin, Zmax);
	glVertex3f(Xmin, Ymax, Zmax);
	//第十一根线
	glVertex3f(Xmax, Ymin, Zmax);
	glVertex3f(Xmax, Ymax, Zmax);
	//第十二根线
	glVertex3f(Xmin, Ymax, Zmax);
	glVertex3f(Xmax, Ymax, Zmax);
	glEnd();
}
AABBbox::~AABBbox(void)
{
}
