#pragma once
#include "components/simple_scene.h"
#include "shapes2D.h"
#include "transf2D.h"

class Character {
protected:
	glm::vec3 position;
	glm::mat3 modelMatrix;
	float primitiveSize;
	float angle;
	float orientateX, orientateY;
public:
	Character(glm::vec3 position, float primitiveSize);
	void virtual render(gfxc::SimpleScene *world, glm::mat3 visMatrix) = 0;
	glm::vec3 virtual getCenter() = 0;
	float virtual getRadius() = 0;
	void move(float tx, float ty);
	glm::vec3 getPosition();
	glm::mat3 getModelMatrix();
	float getPrimitiveSize();
	void orientateCharacter(float x, float y);
	void changeOrientation(int x, int y);
	void updatePosition(glm::vec3 pos);
	void setAngle(float angle);

	float getAngle();
};