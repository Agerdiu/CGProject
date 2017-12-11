#pragma once
#include "reference.h"
#include "objloader.h"
#include "AABBbox.h"
#include <vector>
#include "Vector.h"
#include "Particle.h"
class Model {
public:
	vector<AABBbox>BoxLists;
	AABBbox Box=AABBbox(10000, 10000,10000,-10000,-10000,-10000);
	Model() {};
	void LoadObj(ObjLoader* loader);
	void render();
	GLint lid[2];
	//void loadFromFile(char* objfile, char* mtlfile);
	void DrawModelEdge();
	bool is_In_Model(Point3 p);
	bool is_Collide(Model model);
	void Translate(double x, double y,double z);
	void Scale(double size);
	//void Rotate(double angle,double x,double y,double z);
	void setDir(Vector3 dir);
	Vector3 position = Vector3(0,0,0);
	Vector3 direction = Vector3(0,0,1);
	double size=1;
	ObjLoader* loader;
	CParticle particle;
	void move(Vector3 dir,double speed);
	bool visible=true;
	bool explode = false;
	int explode_time = 0;
	void explosion();
};