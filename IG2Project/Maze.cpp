#include "Maze.h"
#include <cmath>
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

	this->offset = Vector3(-(num_column * block_size / 2), 0, -(num_row * block_size / 2));

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
				bloque = new Hueco(pos, node, mSM);
			}
			else if (type == MURO)
			{
				bloque = new Muro(pos, node, mSM, "cube.mesh");
				node->showBoundingBox(true);
			}
			else if (type == HUECO)
			{
				bloque = new Hueco(pos, node, mSM);
			}
			else
			{
				throw 1;
			}

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
	// colocarlo en el centro
	this->setPosition(this->offset);

	stageFile.close();
};

Vector3 Maze::getHeroStartPosition() const
{
	return hero_position + this->offset; // *El heroe tiene posicion global*
}

void Maze::setHeroPosition(Vector3 vector)
{
	hero_position = vector - this->offset; // Relativo al nodo del laberinto
	//std::cout << hero_position << std::endl;
	//std::cout << "Vector: " << vector << std::endl;
}

bool Maze::can_turn(Vector3 position, Vector3 direction)
{
	
	return false;
}

Vector3 Maze::getHeroPosition() const
{
	return hero_position;
}

void Maze::stepForward(Hero* hero, Casilla* charBlock, Casilla* frontBlock, float dt) {
	if (frontBlock != nullptr && charBlock != nullptr && frontBlock->canPassThrough()) {

		hero->move(hero->getGridOrientation() * Hero::HERO_SPEED * dt);
	}
}

bool Maze::blockCenterReached(Vector3 difference, Vector3 direction) {
	if (direction == Vector3::UNIT_X || direction == Vector3::NEGATIVE_UNIT_X) {
		return abs(difference.x) < 0.5f;
	}
	else if (direction == Vector3::UNIT_Z || direction == Vector3::NEGATIVE_UNIT_Z) {
		return abs(difference.z) < 0.5f;
	}
	else {
		return false;
	}
}

Casilla* Maze::getCasillaAtPosition(Vector3 position)
{
	Vector3 localPos = position - offset;

	int row = std::round(localPos.z / block_size);

	int col = std::round(localPos.x / block_size);

	if (row < 0 || row >= num_row || col < 0 || col >= num_column)
	{
		return nullptr;
	}

	return maze[row][col];
}


void Maze::moveHero(Hero* hero,float dt)
{
	Casilla* charBlock;
	Casilla* inFrontBlock;

	charBlock = this->getCasillaAtPosition(hero->getPosition());

	Vector3 blockCenter = charBlock->getPosition() + this->offset;

	inFrontBlock = this->getCasillaAtPosition(blockCenter + (hero->getGridOrientation() * block_size));

	Vector3 localPos = hero->getPosition() - this->offset;

	Vector3 newPos = localPos + (hero->getGridOrientation() * Hero::HERO_SPEED * dt);


	Vector3 difference = Vector3(newPos.x - charBlock->getPosition().x, 0, newPos.z - charBlock->getPosition().z);

	bool centerReached = blockCenterReached(difference, hero->getGridOrientation());

	if (!hero->getDirectionModified()) {

		if (centerReached) {
			stepForward(hero, charBlock, inFrontBlock, dt);
		}
		else { // si no ha llegado el centro de la casilla actual, mueve hasta el centro
			hero->move(hero->getGridOrientation() * Hero::HERO_SPEED * dt);
		}

		std::cout << "not changed direction" << std::endl;
	}
	else {

		Casilla* nextBlock = this->getCasillaAtPosition(blockCenter + (hero->getCurrentDirection() * block_size)));

		if(nextBlock!=nullptr && nextBlock->canPassThrough() && blockCenterReached(difference, hero->getGridOrientation())) {
			hero->rotateToDirection();
		}
		else if (hero->is180turn()) {
			hero->rotateToDirection();
		}

		else {
			if (centerReached) {
				stepForward(hero, charBlock, inFrontBlock, dt);
			}
			else {
				hero->move(hero->getGridOrientation() * Hero::HERO_SPEED * dt);
			}
		}
	}
}

