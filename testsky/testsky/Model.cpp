#include "Model.h"
#include "objloader.h"
#include <cmath>
#include <Vector.h>
void Model::LoadObj(ObjLoader* Loader) {
	loader = Loader;
	for (int i = 0; i < loader->m_mtls.size(); i++) {
		for (int j = 0; j < loader->m_mtls[i]->triFaces.size() / 9; j++) {
			Point3 p1 = Point3((double)loader->m_mtls[i]->triVertexs[j * 9 + 0], (double)loader->m_mtls[i]->triVertexs[j * 9 + 1],
				(double)loader->m_mtls[i]->triVertexs[j * 9 + 2]);
			Point3 p2 = Point3((double)loader->m_mtls[i]->triVertexs[j * 9 + 3], (double)loader->m_mtls[i]->triVertexs[j * 9 + 4],
				(double)loader->m_mtls[i]->triVertexs[j * 9 + 5]);
			Point3 p3 = Point3((double)loader->m_mtls[i]->triVertexs[j * 9 + 6], (double)loader->m_mtls[i]->triVertexs[j * 9 + 7],
				(double)loader->m_mtls[i]->triVertexs[j * 9 + 8]);
			if (p1.x < Box.Xmin)
				Box.Xmin = p1.x;
			if (p2.x < Box.Xmin)
				Box.Xmin = p2.x;
			if (p3.x < Box.Xmin)
				Box.Xmin = p3.x;
			if (p1.x > Box.Xmax)
				Box.Xmax = p1.x;
			if (p2.x > Box.Xmax)
				Box.Xmax = p2.x;
			if (p3.x > Box.Xmax)
				Box.Xmax = p3.x;
			if (p1.y < Box.Ymin)
				Box.Ymin = p1.y;
			if (p2.y < Box.Ymin)
				Box.Ymin = p2.y;
			if (p3.y < Box.Ymin)
				Box.Ymin = p3.y;
			if (p1.y > Box.Ymax)
				Box.Ymax = p1.y;
			if (p2.y > Box.Ymax)
				Box.Ymax = p2.y;
			if (p3.y > Box.Ymax)
				Box.Ymax = p3.y;
			if (p1.z < Box.Zmin)
				Box.Zmin = p1.z;
			if (p2.z < Box.Zmin)
				Box.Zmin = p2.z;
			if (p3.z < Box.Zmin)
				Box.Zmin = p3.z;
			if (p1.z > Box.Zmax)
				Box.Zmax = p1.z;
			if (p2.z > Box.Zmax)
				Box.Zmax = p2.z;
			if (p3.z > Box.Zmax)
				Box.Zmax = p3.z;
			Triangle triangle = Triangle(p1,p2,p3);
			AABBbox box = AABBbox(triangle);
			BoxLists.push_back(box);
		}
	}
	particle.Init();
	particle.LoadTextures("F:/图形学/专题研讨lab/专题研讨lab/testsky/testsky/testsky/resources/textures/explosion2.bmp");
	lid[0] = glGenLists(1);
	glNewList(lid[0], GL_COMPILE);
	loader->renderObj();
	glEndList();
	lid[1] = glGenLists(1);
	glNewList(lid[1], GL_COMPILE);
	particle.Draw();
	glEndList();
}
/*
void Model::loadFromFile(char* objfile, char* mtlfile) {
	loader.loadFromFile(objfile, mtlfile);
	*this = Model(loader);
	lid = glGenLists(1);
	glNewList(lid, GL_COMPILE);
	loader.renderObj();
	glEndList();
}*/
void Model::DrawModelEdge() {
	//if (visible)
		//for (int i = 0; i < BoxLists.size(); i++)
			//BoxLists[i].DrawAABBBoundingBox();
		Box.DrawAABBBoundingBox();
}
bool Model::is_In_Model(Point3 p)
{
	if (Box.PointIsOrNotInAABBBoundingBox(p) == 1)
		return true;
	return false;
}
bool Model::is_Collide(Model model) {
	if (Box.IsOrNotInterection(&model.Box))
		return true;
	return false;
}
void Model::render() {
	//double angle = acos(direction.normalize().dotProduct(Vector3(0, 0, 1)));
	//Vector3 axis = direction.crossProduct(Vector3(0, 0, 1)).normalize();
	Vector3 dir2 = Vector3(direction.x, 0, direction.z).normalize();
	double angle1 = acos(dir2.dotProduct(Vector3(0, 0, 1)));
	Vector3 axis = direction.crossProduct(dir2).normalize();
	double angle2 = acos(direction.normalize().dotProduct(dir2));
	Vector3 axis2 = dir2.crossProduct(Vector3(0, 0, 1)).normalize();

	if (visible&&!explode) {
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		//glRotated(-1*angle*180/3.1416,axis.x,axis.y,axis.z);
		glRotated(-angle2*180/3.1416, axis.x, axis.y, axis.z);
		glRotated(-angle1 * 180 / 3.1416, axis2.x, axis2.y, axis2.z);
		glScalef(size,size,size);
		//loader.renderObj();
		glCallList(lid[0]);
		glPopMatrix();
	}
	if (visible&&explode) {
		glPushMatrix();
		BoxLists.clear();
		explosion();
		Box.Xmin = 0;
		Box.Xmax = 0;
		Box.Ymin = 0;
		Box.Ymax = 0;
		Box.Zmin = 0;
		Box.Zmax = 0;
		//cout << "explode" << endl;
		//glTranslatef(position.x, position.y, position.z);
		//glRotated(-1 * angle * 180 / 3.1416, axis.x, axis.y, axis.z);
		//glScalef(size, size, size);
		//glCallList(lid);
		glPopMatrix();
	}
}
void Model::explosion() {
	if (explode_time > 1000) {
		visible = false;
		return;
	}
	particle.Draw();
	//glCallList(lid[1]);
	explode_time++;
}
void Model::move(Vector3 direction, double speed) {
	this->direction = direction.normalize();
	position.x -=direction.x * speed*0.0005;
	position.y -= direction.y * speed*0.0005;
	position.z-= direction.z * speed*0.0005;
	/*for (int i = 0; i < BoxLists.size(); i++) {
		BoxLists[i].Xmax -= direction.x * speed*0.00005;
		BoxLists[i].Xmin -= direction.x * speed*0.00005;
		BoxLists[i].Ymax -= direction.y * speed*0.00005;
		BoxLists[i].Ymin -= direction.y * speed*0.00005;
		BoxLists[i].Zmax -= direction.z * speed*0.00005;
		BoxLists[i].Zmin -= direction.z * speed*0.00005;
	}*/
	Box.Xmax -= direction.x * speed*0.0005;
	Box.Xmin -= direction.x * speed*0.0005;
	Box.Ymax -= direction.y * speed*0.0005;
	Box.Ymin -= direction.y * speed*0.0005;
	Box.Zmax -= direction.z * speed*0.0005;
	Box.Zmin -= direction.z * speed*0.0005;
	particle.SetPosition(position.x, position.y, position.z);
}
void Model::Translate(double x,double y,double z) {
	position.x = position.x +x;
	position.y = position.y+y;
	position.z = position.z +z;
	/*for (int i = 0; i < BoxLists.size(); i++) {
		BoxLists[i].Xmax += x;
		BoxLists[i].Xmin += x;
		BoxLists[i].Ymax += y;
		BoxLists[i].Ymin += y;
		BoxLists[i].Zmax += z;
		BoxLists[i].Zmin += z;
	}*/
	Box.Xmax += x;
	Box.Xmin += x;
	Box.Ymax += y;
	Box.Ymin += y;
	Box.Zmax += z;
	Box.Zmin += z;
	particle.SetPosition(position.x, position.y, position.z);
}
void Model::Scale(double Size) {
	/*for (int i = 0; i < BoxLists.size(); i++) {
		BoxLists[i].Xmax *= size;
		BoxLists[i].Xmin *= size;
		BoxLists[i].Ymax *= size;
		BoxLists[i].Ymin *= size;
		BoxLists[i].Zmax *= size;
		BoxLists[i].Zmin *= size;
	}*/
	/*for (int i = 0; i < loader.m_mtls.size(); i++)
		for (int j = 0; j < loader.m_mtls[i]->triNum * 9; j++)
			loader.m_mtls[i]->triVertexs[j] *= size;*/
	size = Size;
	Box.Xmax *= Size;
	Box.Xmin *= Size;
	Box.Ymax *= Size;
	Box.Ymin *= Size;
	Box.Zmax *= Size;
	Box.Zmin *= Size;
}
/*
void Model::Rotate(double angle, double x, double y, double z) {
	Vector3 dir(x,y,z);
	dir=dir.normalize();
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);
	double Rotate[3][3];
	Rotate[0][0] = (cosTheta + (1 - cosTheta) * x * x);
	Rotate[0][1] = ((1 - cosTheta) * x * y - z * sinTheta);
	Rotate[0][2]=((1 - cosTheta) * x * z + y * sinTheta);

	Rotate[1][0]= ((1 - cosTheta) * x * y + z * sinTheta);
	Rotate[1][1] = (cosTheta + (1 - cosTheta) * y * y);
	Rotate[1][2] = ((1 - cosTheta) * y * z - x * sinTheta);

	Rotate[2][0] = ((1 - cosTheta) * x * z - y * sinTheta);
	Rotate[2][1] = ((1 - cosTheta) * y * z + x * sinTheta);
	Rotate[2][2] = (cosTheta + (1 - cosTheta) * z * z);

}*/
void Model::setDir(Vector3 dir) {
	direction = dir.normalize();
	
}
