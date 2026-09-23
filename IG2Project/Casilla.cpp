#include "Casilla.h"

Casilla::Casilla(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh) : IG2Object(initPos, node, sceneMng, mesh)
{

}

Casilla::Casilla(Vector3 initPos, SceneNode* node, SceneManager* sceneMng) : IG2Object(initPos, node, sceneMng)
{

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
