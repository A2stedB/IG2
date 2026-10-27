#pragma once
#include "IG2Object.h"
#include <vector>
#include <string>
#include "Casilla.h"
#include "Hero.h"
#include "Villano.h"

class Hero;

class Maze : public IG2Object
{
public:
	Maze(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh);

	void createMaze(std::string stageFileName);

	Vector3 getHeroStartPosition() const;
	// Hacer un metodo para add bloques

	void setHeroPosition(Vector3 vector);

	bool getAvailableDirection(Vector3 position, Vector3 direction); // Para luego los enemigos

	Vector3 getHeroPosition() const;
	Vector3 getVillanoStartPosition() const;

	bool is_intersection() const;

	Casilla* getCasillaAtPosition(Vector3 position);

	void moveCharacter(Character* hero, float dt);

	void stepForward(Character* hero, Casilla* charBloack, Casilla* frontBlock, float dt);

	bool blockCenterReached(Vector3 difference, Vector3 direction);

	void update(float dt,Hero* hero, Villano* villano);

	//bool is_in_a_intersection(Character* character);

	Ogre::Light::LightTypes getHeroLightType() const
	{
		return heroLightType;
	}

private:
	bool is_valid_index(int row,int col);

private:
	Vector3 offset;
	int num_row;
	int num_column;
	Vector3 hero_position;
	Vector3 villano_position;
	std::vector<std::vector<Casilla*>> maze;
	Ogre::SceneNode* floorNode;

	//Ogre::SceneNode* mHeroSpotLightNode = nullptr;
	Ogre::Light::LightTypes heroLightType;
};

// Una clase casilla


