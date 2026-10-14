#include "Casilla.h"

Casilla::Casilla(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh) : IG2Object(initPos, node, sceneMng, mesh)
{

}

Casilla::Casilla(Vector3 initPos, SceneNode* node, SceneManager* sceneMng) : IG2Object(initPos, node, sceneMng)
{

}

bool Casilla::canPassThrough()
{
	return can_move;
}

Hueco::Hueco(Vector3 initPos, SceneNode* node, SceneManager* sceneMng) :
	Casilla(initPos,node,sceneMng)
{
	can_move = true;
}

Muro::Muro(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh) :
	Casilla(initPos, node, sceneMng,mesh)
{
	can_move = false;
}

BloqueInvisible::BloqueInvisible(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh)
	: Muro(initPos, node, sceneMng, mesh)
{

}

void BloqueInvisible::update(float dt)
{
	current_time += dt;
	if (current_time >= interval) 
	{
		is_visible = !is_visible;
		if (!is_visible) {
			this->mNode->detachObject(this->entity);
		}
		else {
			this->mNode->attachObject(this->entity);
		}
		current_time = 0;
	}
}

BloqueFalso::BloqueFalso(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh)
	: Casilla(initPos,node,sceneMng,mesh)
{
	can_move = true;
}
