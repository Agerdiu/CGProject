#pragma once
#include "reference.h"
class AABBbox {
public:
	AABBbox(void);
	AABBbox(Point3 pointMin, Point3 pointMax);
	AABBbox(double m_Xmin, double m_Ymin, double m_Zmin, double m_Xmax, double m_Ymax, double m_Zmax);
	AABBbox(Triangle a);
	bool IsOrNotInterection(AABBbox *a);
	bool PointIsOrNotInAABBBoundingBox(Point3 pt);
	Point3 GetAABBBoxCeter();
	bool IsOrNotEmpty();
	double GetXLength();
	double GetYLength();
	double GetZLength();
	double GetVolume();
	void DrawAABBBoundingBox();
	~AABBbox();
	double Xmin ;
	double Xmax;
	double Ymin;
	double Ymax;
	double Zmin;
	double Zmax;
};