#pragma once
#include "Character.h"

class Obstacle : public Character {
public:
	float sx, sy, width, height;
	Obstacle(glm::vec3 pos, float primitiveSize, float sx, float sy);
	void render(gfxc::SimpleScene* world, glm::mat3 visMatrix);
	glm::vec3 getCenter() override;
	float getRadius() override;
	float getWidth(glm::mat3 visMatrix);
	float getHeight(glm::mat3 visMatrix);
};
