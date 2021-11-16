#include "Obstacle.h"
void Obstacle::render(gfxc::SimpleScene* world, glm::mat3 visMatrix) {

	glm::vec3 pos = getPosition();
	glm::mat3 modelMatrix = getModelMatrix();
	modelMatrix = modelMatrix
		* transf2D::Scale(sx, sy);

	world->RenderMesh2D(world->meshes["square_full"], visMatrix * modelMatrix, glm::vec3(0, 0.3, 0));
}

Obstacle::Obstacle(glm::vec3 pos, float primitiveSize, float sx, float sy) : Character::Character(pos, primitiveSize) {
	this->sx = sx;
	this->sy = sy;
	width = sx * primitiveSize;
	height = sy * primitiveSize;
}

float Obstacle::getRadius() {
	return 0;
}

glm::vec3 Obstacle::getCenter() {
	return getModelMatrix() * glm::vec3(width / 2, height / 2, 1);
}

float Obstacle::getWidth(glm::mat3 visMatrix) {
	return width;
}

float Obstacle::getHeight(glm::mat3 visMatrix) {
	return height;
}
