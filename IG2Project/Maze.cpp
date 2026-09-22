#include "Maze.h"
#include <fstream>

constexpr const char MURO = 'x';
constexpr const char HUECO = 'o';
const int block_size = 100;

Casilla::Casilla(bool movable, Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh) : IG2Object(initPos,node,sceneMng,mesh)
{
	can_move = movable;
}

Maze::Maze(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh) : IG2Object(initPos,node,sceneMng,mesh)
{
}

void Maze::createMaze(std::string stageFileName)
{
	ifstream stageFile(stageFileName);

	if (!stageFile)
		throw 1;

	stageFile >> num_row;
	stageFile >> num_column;

	maze.resize(num_row);

	for (int i = 0; i < num_row;++i) {

		for (int j = 0; j < num_column; ++j) {
			char type;
			stageFile >> type;

			Vector3 pos(j * block_size, 0, i * block_size);

			SceneNode* node = mSM->getRootSceneNode()->createChildSceneNode();

			if (type == MURO) {
				
				Casilla* bloque = new Casilla(false, pos, node, mSM, "cube.mesh");
				maze[i].push_back(bloque);
			}
			else if (type == HUECO) {

				Casilla* bloque = new Casilla(true, pos, node, mSM, "cube.mesh");
				bloque->setVisible(false);
				maze[i].push_back(bloque);
			}
			else
				throw 1;
		}
	}


	stageFile.close();
}
