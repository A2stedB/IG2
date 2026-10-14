#pragma once
#include "IG2Object.h"

class Casilla : public IG2Object
{
public:
	Casilla() {};
	Casilla(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh);
	Casilla(Vector3 initPos, SceneNode* node, SceneManager* sceneMng);
	bool canPassThrough();
	virtual void update(float dt){};
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

class BloqueInvisible : public Muro 
{
public:
	BloqueInvisible(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh);
	void update(float dt) override;
private:
	bool is_visible{ true };
	float current_time{ 0.0f };
	float interval{3.0f};
};

class BloqueFalso : public Casilla
{
public:
    BloqueFalso(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh);
};

class BreakableBlock : public Muro
{
public:
	BreakableBlock(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh);
};
