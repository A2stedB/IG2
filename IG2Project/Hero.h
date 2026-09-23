#pragma once
#include "IG2Object.h"
class Hero : public IG2Object
{
public:
	Hero(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh);

	Vector3 getCurrentDirection() const;
	void setDirection(Vector3 vector);
private:
	Vector3 current_direction{ 0,0,-1 };
};

