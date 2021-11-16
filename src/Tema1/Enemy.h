#pragma once
#include "Character.h"
class Enemy : public Character {
public:
	using Character::Character;
	void render(gfxc::SimpleScene* world, glm::mat3 visMatrix);
	glm::vec3 getCenter() override;
	float getRadius() override;
private:
	float mouseX, mouseY;
	
};
