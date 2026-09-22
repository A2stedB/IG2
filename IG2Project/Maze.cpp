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

	maze.reserve(3);

	for (int i = 0; i < num_row;++i) {
		for (int j = 0; j < num_column;++i) {
			char type;
			stageFile >> type;

			if (type == MURO) {
				maze[i].push_back(Casilla(false, Vector3{0,0,0},mNode,mSM,"cube.mesh"));
			}
			else if (type == HUECO) {
				maze[i].push_back(Casilla(true, Vector3{ 0,0,0 }, mNode, mSM, "cube.mesh"));
				maze[i][j].setVisible(false);
			}
			else
				throw 1;
		}
	}

	stageFile.close();
}
