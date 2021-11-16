#include "Bullet.h"

#include <iostream>
void Bullet::render(gfxc::SimpleScene* world, glm::mat3 visMatrix) {
	glm::vec3 pos = getPosition();
	glm::mat3 modelMatrix = getModelMatrix();
	modelMatrix = modelMatrix
		* transf2D::Translate(primitiveSize / 2, primitiveSize / 2)
		* transf2D::Rotate(-getAngle())
		* transf2D::Scale(scaleX, scaleY)
		* transf2D::Translate(-primitiveSize / 2, -primitiveSize / 2);
	
	
	//render bullet
	world->RenderMesh2D(world->meshes["square_full"], visMatrix * modelMatrix, glm::vec3(0, 2, 0));

	 modelMatrix = getModelMatrix();
	 modelMatrix = modelMatrix
		 * transf2D::Translate(primitiveSize / 2, primitiveSize / 2)
		 * transf2D::Scale(scaleX, scaleY);

	world->RenderMesh2D(world->meshes["circle_empty"], visMatrix * modelMatrix, glm::vec3(0, 2, 0));
}

Bullet::Bullet(glm::vec3 pos, float primitiveSize, float angle) : Character::Character(pos, primitiveSize) {
	scaleX = scaleY = 0.6;
	traversedRange = 0;
	setAngle(angle);
}

bool Bullet::checkOutOfBounds(int left, int right, int top, int bottom) {
	glm::vec3 cen = getCenter();

	float radius = getRadius();
	if (cen.x + radius >= right ||
		cen.x - radius <= left ||
		cen.y + radius >= top ||
		cen.y - radius <= bottom) {
		return 1;
	}

	return 0;
}

glm::vec3 Bullet::getCenter() {
	return getModelMatrix() * transf2D::Translate(primitiveSize / 2, primitiveSize / 2)
		* transf2D::Scale(scaleX, scaleY) * glm::vec3(0, 0, 1);
}

float Bullet::getRadius() {
	return primitiveSize / 2;
}

void Bullet::incrementTraversedRange(float dist) {
	traversedRange += dist;
}

float Bullet::getTraversedRange() {
	return traversedRange;
}
