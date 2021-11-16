#include "Player.h"
using namespace std;

void Player::render(gfxc::SimpleScene *world, glm::mat3 visMatrix) {

	glm::vec3 pos = getPosition();
	glm::mat3 modelMatrix = getModelMatrix();
	
	//orientate after mouse
	modelMatrix *= transf2D::Rotate(-getAngle());

	//center circle
	world->RenderMesh2D(world->meshes["circle_full"], world->shaders["VertexColor"], visMatrix * modelMatrix);

	//right leg
	int radius = getPrimitiveSize();
	
	modelMatrix *= transf2D::Rotate(RADIANS(40))
		* transf2D::Translate(0, -radius)
		* transf2D::Scale(0.4, 0.4);


	world->RenderMesh2D(world->meshes["circle_full"], visMatrix * modelMatrix, glm::vec3(1, 0, 0));
	world->RenderMesh2D(world->meshes["circle_empty"], visMatrix * modelMatrix, glm::vec3(0, 0, 0));


	//left leg
	modelMatrix = getModelMatrix();

	//orientate after mouse
	modelMatrix *= transf2D::Rotate(-getAngle());

	modelMatrix *= transf2D::Rotate(RADIANS(-40))
		* transf2D::Translate(0, -radius)
		* transf2D::Scale(0.4, 0.4);
	world->RenderMesh2D(world->meshes["circle_full"], visMatrix * modelMatrix, glm::vec3(1, 0, 0));
	world->RenderMesh2D(world->meshes["circle_empty"], visMatrix * modelMatrix, glm::vec3(0, 0, 0));
	
}

glm::vec3 Player::getCenter() {
	return getModelMatrix() * glm::vec3(0, 0, 1);
}

float Player::getRadius() {
	return primitiveSize;
}
