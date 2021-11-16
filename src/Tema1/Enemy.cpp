#include "Enemy.h"
void Enemy::render(gfxc::SimpleScene* world, glm::mat3 visMatrix) {

	glm::vec3 pos = getPosition();
	glm::mat3 modelMatrix = getModelMatrix();

	//orientate after mouse
	modelMatrix *= transf2D::Rotate(-getAngle())  * transf2D::Scale(1.5, 1.5) * transf2D::Translate(-primitiveSize / 2, -primitiveSize / 2);

	//center square
	world->RenderMesh2D(world->meshes["square_full"], visMatrix * modelMatrix, glm::vec3(1, 0, 0));
	world->RenderMesh2D(world->meshes["square_empty"], visMatrix * modelMatrix, glm::vec3(0, 0, 0));

	//right leg

	modelMatrix *= transf2D::Translate(-primitiveSize / 2, -primitiveSize / 2)
		* transf2D::Scale(0.5, 0.5)
		* transf2D::Translate(primitiveSize / 2, primitiveSize / 2);
	world->RenderMesh2D(world->meshes["square_full"], visMatrix * modelMatrix, glm::vec3(1, 0, 0));
	world->RenderMesh2D(world->meshes["square_empty"], visMatrix * modelMatrix, glm::vec3(0, 0, 0));



	//left leg
	modelMatrix = getModelMatrix();

	//orientate after mouse
	modelMatrix *= transf2D::Rotate(-getAngle());

	modelMatrix *= transf2D::Scale(1.5, 1.5) *  transf2D::Translate(0, -primitiveSize) * transf2D::Translate(primitiveSize / 2, primitiveSize / 2)
		* transf2D::Scale(0.5, 0.5)
		* transf2D::Translate(-primitiveSize / 2, -primitiveSize / 2);

	world->RenderMesh2D(world->meshes["square_full"], visMatrix * modelMatrix, glm::vec3(1, 0, 0));
	world->RenderMesh2D(world->meshes["square_empty"], visMatrix * modelMatrix, glm::vec3(0, 0, 0));

	
}

glm::vec3 Enemy::getCenter() {
	return  getModelMatrix() * glm::vec3(0, 0, 1);
}

float Enemy::getRadius() {
	return primitiveSize;
}