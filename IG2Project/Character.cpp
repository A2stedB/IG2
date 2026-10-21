#include "Character.h"

Character::Character(Vector3 initPos, SceneNode * node, SceneManager * sceneMng, String mesh, float speed) :
	IG2Object(initPos, node, sceneMng, mesh),speed(speed) {
	mNode->showBoundingBox(true);
}

Character::Character(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, float speed) :
	IG2Object(initPos, node, sceneMng), speed(speed) {
	mNode->showBoundingBox(true);
}


Vector3 Character::getCurrentDirection() const
{
	return current_direction;
}

void Character::setDirection(Vector3 vector)
{
	current_direction = vector;
}


bool Character::getDirectionModified() {

	return current_direction != getGridOrientation();
}
void Character::rotateToDirection()
{
	Quaternion rotation =
		getGridOrientation().getRotationTo(current_direction);

	rotate(rotation);
}

float Character::getSpeed()const {
	return speed;
}

bool Character::is180turn() {
	return current_direction == -getGridOrientation();
}