#include "Character.h"

Character::Character(glm::vec3 pos, float primitiveSize) {
	position = pos;
	modelMatrix = transf2D::Translate(pos.x, pos.y);
	this->primitiveSize = primitiveSize;
	angle = 0;
}

glm::vec3 Character::getPosition() {
	return this->position;
}

glm::mat3 Character::getModelMatrix() {
	return this->modelMatrix;
}

float Character::getPrimitiveSize() {
	return this->primitiveSize;
}

void Character::move(float tx, float ty) {
	position.x += tx;
	position.y += ty;
	modelMatrix *= transf2D::Translate(tx, ty);
}

void Character::orientateCharacter(float x, float y) {
	angle = atan2(x - orientateX, y - orientateY);	
}

void Character::changeOrientation(int x, int y) {
	orientateX = x;
	orientateY = y;
}

float Character::getAngle() {
	return angle;
}

void Character::setAngle(float angle) {
	this->angle = angle;
}

void Character::updatePosition(glm::vec3 pos) {
	position = pos;
}

