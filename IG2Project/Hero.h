#pragma once
#include "IG2Object.h"
class Hero : public IG2Object
{
public:
	static float HERO_SPEED;

	Hero(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh);

	Vector3 getCurrentDirection() const;
	void setDirection(Vector3 vector);
	bool getDirectionModified();
	void rotateToDirection();

	bool is180turn();

private:
	Vector3 current_direction{ 1,0,0 };

	bool direction_modified{ false };
};

