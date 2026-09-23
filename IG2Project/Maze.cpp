#include "Maze.h"
#include <fstream>

constexpr const char MURO = 'x';
constexpr const char HUECO = 'o';
constexpr const char HERO = 'h';

const int block_size = 20;

Maze::Maze(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh) : IG2Object(initPos,node,sceneMng)
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

	for (int i = 0; i < num_row; ++i) {

		for (int j = 0; j < num_column; ++j) {
			char type;
			stageFile >> type;

			Vector3 pos(j * block_size, 0, i * block_size);

			SceneNode* node = this->createChildSceneNode();

			Casilla* bloque;

			if (type == HERO)
			{
				hero_position = pos;
			}
			else {
				if (type == MURO)
				{
					bloque = new Muro(pos, node, mSM, "cube.mesh");
					node->showBoundingBox(true);
				}
				else if (type == HUECO) {

					bloque = new Hueco(pos, node, mSM);
				}
				else
					throw 1;

				//redimensionar escalando
				Vector3 size = bloque->calculateBoxSize();
				float propX, propY, propZ;

				propX = block_size / size.x;
				propY = block_size / size.y;
				propZ = block_size / size.z;

				bloque->setScale(Vector3(propX, propY, propZ));

				maze[i].push_back(bloque);
			}

		}
	}
	// colocarlo en el centro
	this->setPosition(Vector3(-(num_column * block_size / 2), 0, -(num_row * block_size / 2)));

	stageFile.close();
};

Vector3 Maze::getHeroStartPosition() const
{
	return hero_position + Vector3(-(num_column * block_size / 2), 0, -(num_row * block_size / 2));
}
