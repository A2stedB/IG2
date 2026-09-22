#pragma once
#include "IG2Object.h"
#include <vector>
#include <string>

class Casilla : public IG2Object
{
public:
	Casilla(bool movable, Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh);

private:
	bool can_move;
};


class Maze : public IG2Object
{
public:
	Maze(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh);

	void createMaze(std::string stageFileName);

	// Hacer un metodo para add bloques

private:
	int num_row;
	int num_column;
	std::vector<std::vector<Casilla>> maze;
};

