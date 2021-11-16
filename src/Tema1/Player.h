#pragma once
#include "Character.h"

class Player : public Character {
public:
	using Character::Character;
	void render(gfxc::SimpleScene *world, glm::mat3 visMatrix);
	glm::vec3 getCenter();
	float getRadius();
private:
	float mouseX, mouseY;
};
