#pragma once
#include "IG2Object.h"
#include <vector>
#include <string>
#include "Casilla.h"

class Maze : public IG2Object
{
public:
	Maze(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh);

	void createMaze(std::string stageFileName);

	Vector3 getHeroStartPosition() const;
	// Hacer un metodo para add bloques

	void setHeroPosition(Vector3 vector);

	bool can_turn(Vector3 position, Vector3 direction); // Para luego los enemigos

	Vector3 getHeroPosition() const;
	
	bool is_intersection() const;

	void moveHero();
private:
	Vector3 offset;
	int num_row;
	int num_column;
	Vector3 hero_position;

	std::vector<std::vector<Casilla*>> maze;
};

// Una clase casilla


