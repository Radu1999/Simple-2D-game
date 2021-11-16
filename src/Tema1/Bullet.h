#pragma once
#include "Character.h"

class Bullet : public Character {
private:
	float scaleX, scaleY;
	float collisionRadius;
	float traversedRange;
public:
	Bullet(glm::vec3 pos, float primitiveSize, float angle);
	void render(gfxc::SimpleScene* world, glm::mat3 visMatrix);
	bool checkOutOfBounds(int left, int right, int top, int bottom);
	glm::vec3 getCenter() override;
	float getRadius() override;
	void incrementTraversedRange(float dist);
	float getTraversedRange();

};
