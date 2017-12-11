#pragma once
#include "reference.h"
#include "Model.h"
class Bullet {
public:
	Point3 position = {0,0,0};
	Vec3 direction = {0,0,-1};
	int Speed=1;
	int power;
	int duration = 0;
	Bullet() {};
	Bullet(Point3 pos, Vec3 dir) :position(pos) {
		direction.x = dir.x / Vector::getMod(dir);
		direction.y = dir.y / Vector::getMod(dir);
		direction.z = dir.z / Vector::getMod(dir);
	};
	Bullet(Point3 pos, Vec3 dir,int sp) :position(pos), direction(dir),Speed(sp) {};
	Bullet(Point3 pos, Vec3 dir,int sp,int p) :position(pos), direction(dir), Speed(sp), power(p) {};
	void shot();
	bool is_hit(Model* model);
};