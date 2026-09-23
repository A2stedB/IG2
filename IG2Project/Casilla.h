#pragma once
#include "IG2Object.h"

class Casilla : public IG2Object
{
public:
	Casilla() {};
	Casilla(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh);
	Casilla(Vector3 initPos, SceneNode* node, SceneManager* sceneMng);
protected:
	bool can_move;
};

class Hueco : public Casilla 
{
public:
	Hueco(Vector3 initPos, SceneNode* node, SceneManager* sceneMng);
};

class Muro : public Casilla 
{
public:
	Muro(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh);
};
