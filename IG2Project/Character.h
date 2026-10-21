#pragma once
#include "IG2Object.h"
class Character : public IG2Object
{

public:
	Character(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh, float speed);
	Character(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, float speed);
	Vector3 getCurrentDirection() const;
	void setDirection(Vector3 vector);
	bool getDirectionModified();
	void rotateToDirection();
	float getSpeed()const;
	bool is180turn();

protected:
	Vector3 current_direction{ 1,0,0 };
	float speed;
	bool direction_modified{ false };
};

