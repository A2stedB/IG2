#include "Maze.h"
#include <cmath>
#include <fstream>
#include "Character.h"

constexpr const char MURO = 'x';
constexpr const char HUECO = 'o';
constexpr const char HERO = 'h';
constexpr const char FAKE = 'f';
constexpr const char INVISIBLE = 'i';
constexpr const char VILLANO = 'v';

const int block_size = 20;

Maze::Maze(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh) : IG2Object(initPos, node, sceneMng)
{
    floorNode = this->createChildSceneNode();
}

void Maze::createMaze(std::string stageFileName)
{
    ifstream stageFile(stageFileName);

    if (!stageFile)
        throw 1;

    stageFile >> num_row;
    stageFile >> num_column;

    std::string floor_material{};
    std::string wall_material{};
    std::string light_type;

    stageFile >> floor_material >> wall_material>> light_type;

    if (light_type == "directional") {
        heroLightType = Ogre::Light::LT_DIRECTIONAL;
    }
    else if (light_type == "spot") {
        heroLightType = Ogre::Light::LT_SPOTLIGHT;
    }
    else if (light_type == "point") {
        heroLightType = Ogre::Light::LT_POINT;
    }
    else throw 1;

    //mHeroSpotLightNode = mSM->getRootSceneNode()->createChildSceneNode("HeroLight");
    //Light* luz = mSM->createLight("HeroLight");
    //luz->setType(Ogre::Light::LT_SPOTLIGHT);

    Ogre::MeshPtr floor_mesh = Ogre::MeshManager::getSingleton().createPlane("floor", /*Nombre de la malla creada*/
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, /*El grupo que pertenece este "recurso"*/
        Plane(Vector3::UNIT_Y, 0), /*El plano y su normal*/
        1500, 1500, /*Ancho y alto*/
        50, 50, /*El numero de segmentos en x e y */
        true, /*Habilitar la normal normal*/
        1, /*wtf el numero de las coordenadas de textura*/
        1, 1, /*Los tiles en el que se divide la malla en x e y*/
        Vector3::UNIT_Z);

    Ogre::Entity* floorEntity = mSM->createEntity(floor_mesh);
    floorNode->attachObject(floorEntity);
    floorEntity->setMaterialName(floor_material);
    maze.resize(num_row);

    this->offset = Vector3(-(num_column * block_size / 2), 0, -(num_row * block_size / 2));
    floorNode->translate(-offset - Vector3(0, block_size / 2, 0));

    for (int i = 0; i < num_row; ++i)
    {

        for (int j = 0; j < num_column; ++j)
        {
            char type;
            stageFile >> type;

            Vector3 pos(j * block_size, 0, i * block_size);

            SceneNode* node = this->createChildSceneNode();

            Casilla* bloque;

            switch (type)
            {
            case HERO:
                hero_position = pos;
                bloque = new Hueco(pos, node, mSM);
                break;
            case MURO:
                bloque = new Muro(pos, node, mSM, "cube.mesh");
                node->showBoundingBox(true);
                break;
            case HUECO:
                bloque = new Hueco(pos, node, mSM);
                break;
            case INVISIBLE:
                bloque = new BloqueInvisible(pos, node, mSM, "cube.mesh");
                break;
            case FAKE:
                bloque = new BloqueFalso(pos, node, mSM, "cube.mesh");
                break;
            case VILLANO:
                villano_position = pos;
                bloque = new Hueco(pos, node, mSM);
                break;
            default:
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

            if (type != HUECO && type != HERO && type != VILLANO)
            {
                bloque->setMaterialName(wall_material);
            }

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

Vector3 Maze::getVillanoStartPosition() const
{
    return villano_position + this->offset;
}

void Maze::setHeroPosition(Vector3 vector)
{
    hero_position = vector - this->offset; // Relativo al nodo del laberinto
    //std::cout << hero_position << std::endl;
    //std::cout << "Vector: " << vector << std::endl;
}

Vector3 Maze::getHeroPosition() const
{
    return hero_position;
}

void Maze::stepForward(Character* hero, Casilla* charBlock, Casilla* frontBlock, float dt)
{
    if (frontBlock != nullptr && charBlock != nullptr && frontBlock->canPassThrough())
    {

        hero->move(hero->getGridOrientation() * hero->getSpeed() * dt);
    }
}

bool Maze::blockCenterReached(Vector3 difference, Vector3 direction)
{
    if (direction == Vector3::UNIT_X || direction == Vector3::NEGATIVE_UNIT_X)
    {
        return abs(difference.x) < 0.5f;
    }
    else if (direction == Vector3::UNIT_Z || direction == Vector3::NEGATIVE_UNIT_Z)
    {
        return abs(difference.z) < 0.5f;
    }
    else
    {
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


void Maze::moveCharacter(Character* character, float dt)
{
    Casilla* charBlock;
    Casilla* inFrontBlock;

    charBlock = this->getCasillaAtPosition(character->getPosition());

    if (charBlock == nullptr)
        return;

    Vector3 blockCenter = charBlock->getPosition() + this->offset;

    inFrontBlock = this->getCasillaAtPosition(blockCenter + (character->getGridOrientation() * block_size));

    Vector3 localPos = character->getPosition() - this->offset;

    Vector3 newPos = localPos + (character->getGridOrientation() * character->getSpeed() * dt);

    Vector3 difference = Vector3(newPos.x - charBlock->getPosition().x, 0, newPos.z - charBlock->getPosition().z);

    bool centerReached = blockCenterReached(difference, character->getGridOrientation());

    if (!character->getDirectionModified())
    {

        if (centerReached)
        {
            stepForward(character, charBlock, inFrontBlock, dt);
        }
        else
        { // si no ha llegado el centro de la casilla actual, mueve hasta el centro
            character->move(character->getGridOrientation() * character->getSpeed() * dt);
        }

    }

    else
    {

        Casilla* nextBlock = this->getCasillaAtPosition(blockCenter + (character->getCurrentDirection() * block_size));

        if (nextBlock != nullptr && nextBlock->canPassThrough() && blockCenterReached(difference, character->getGridOrientation()))
        {
            character->setPosition(blockCenter);
            character->rotateToDirection();
        }
        else if (character->is180turn() && centerReached)
        {
            character->setPosition(blockCenter);
            character->rotateToDirection();
        }

        else
        {
            if (centerReached)
            {
                stepForward(character, charBlock, inFrontBlock, dt);
            }
            else
            {
                character->move(character->getGridOrientation() * character->getSpeed() * dt);
            }
        }
    }
}

void Maze::update(float dt, Hero* hero, Villano* villano)
{
    moveCharacter(hero, dt);
    moveCharacter(villano, dt);

    for (int i = 0; i < num_row; ++i)
    {
        for (int j = 0; j < num_column; ++j)
        {
            maze[i][j]->update(dt);
        }
    }
}

//bool Maze::is_in_a_intersection(Character* character)
//{
//	Casilla* charBlock;
//	Casilla* inFrontBlock;
//
//	charBlock = this->getCasillaAtPosition(character->getPosition());
//
//	Vector3 localPos = character->getPosition() - offset;
//
//	int row = std::round(localPos.z / block_size);
//
//	int col = std::round(localPos.x / block_size);
//
//	Vector3 orientation = character->getGridOrientation();
//
//	return false;
//}

bool Maze::is_valid_index(int row, int col)
{
    return row < 0 || row >= num_row || col < 0 || col >= num_column;
}

