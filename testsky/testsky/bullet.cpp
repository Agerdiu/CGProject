#include "bullet.h"
#include "reference.h"
#include "glut\glut.h"
void Bullet::shot() {
	glDisable(GL_BLEND);
	GLUquadricObj *g_text = gluNewQuadric();
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(position.x ,position.y,position.z);
	glScalef(1, 1, 1);
	gluSphere(g_text, 0.1f, 15, 8);
	glPopMatrix();
	duration++;
	position.x = direction.x*Speed*duration*0.0001 + position.x;
	position.y = direction.y*Speed*duration*0.0001 + position.y;
	position.z = direction.z*Speed*duration*0.0001 + position.z;
}
bool Bullet::is_hit(Model* model) {
	/*for (int i = 0; i < model.BoxLists.size(); i++) {
		if (model.BoxLists[i].PointIsOrNotInAABBBoundingBox(position))
			return true;
	}*/
	if (model->Box.PointIsOrNotInAABBBoundingBox(position))
		return true;
	return false;
}