#include "IG2Project.h"
#include "Maze.h"
#include "Bomb.h"
#include <OgreTrays.h>
using namespace std;
using namespace Ogre;


Quaternion IG2Project::getQuaternionForNewDirection() const
{
    return Quaternion(mHero->getCurrentDirection().getRotationTo(nextDir));
}

void IG2Project::showCurrentInfo()
{
    std::stringstream ss;
    // Caption
    // Game status
    ss.str(std::string());
    ss << "Lives: " << mHero->getLives() << endl;
    ss << "Points: " << mHero->getPoints() << endl;
    mTextbox->setText(ss.str());
}


void IG2Project::frameRendered(const Ogre::FrameEvent& evt)
{
    const float dt = evt.timeSinceLastFrame;

    if (!in_game)
    {
        if (mHero != nullptr) mHero->ani_update(dt);
        return;
    }

    if (collisionCooldown > 0.0f)
        collisionCooldown -= dt;

    if (mHero != nullptr) mHero->update(dt);

    if (mMaze != nullptr)
        mMaze->update(dt, mHero, villanoAlive ? mVillano : nullptr);

    if (villanoAlive && mVillano != nullptr)
        mVillano->update(dt);

    // Avanzamos las bombas activas sin modificar el contenedor durante un for-range.
    for (auto it = activatedBombs.begin(); it != activatedBombs.end(); )
    {
        Bomb* bomb = *it;
        bomb->update(dt);

        if (bomb->isExploded())
        {
            // Maze crea el humo y comprueba los BLOQUES afectados en este instante.
            ExplosionResult result = mMaze->explodeBomb(
                bomb->getPosition(), mHero, villanoAlive ? mVillano : nullptr);

            it = activatedBombs.erase(it);
            notActivatedBombs.push_back(bomb); // Se reutilizara con otra barra espaciadora.

            if (result.villanoHit && mVillano != nullptr && villanoAlive)
            {
                villanoAlive = false;
                mVillano->setVisible(false);
            }

            if (result.heroHit && mHero != nullptr && collisionCooldown <= 0.0f)
            {
                mHero->loseLife();
                if (mHero->getLives() <= 0)
                {
                    getRoot()->queueEndRendering();
                    return;
                }
                mHero->setPosition(mMaze->getHeroStartPosition());
                collisionCooldown = 1.0f;
            }
        }
        else
        {
            ++it;
        }
    }

    // Colision habitual entre personajes (independiente de las explosiones).
    if (collisionCooldown <= 0.0f && mHero != nullptr &&
        villanoAlive && mVillano != nullptr &&
        mHero->getAABB().intersects(mVillano->getAABB()))
    {
        mHero->loseLife();
        if (mHero->getLives() <= 0)
        {
            getRoot()->queueEndRendering();
            return;
        }
        mHero->setPosition(mMaze->getHeroStartPosition());
        collisionCooldown = 1.0f;
    }

    if (mTextbox != nullptr) showCurrentInfo();
}

void IG2Project::spawnBomb()
{
    if (mHero == nullptr || mMaze == nullptr) return;
    if (activatedBombs.size() >= static_cast<size_t>(mHero->getMaxBomb())) return;

    Casilla* block = mMaze->getCasillaAtPosition(mHero->getPosition());
    if (block == nullptr || !block->canPassThrough()) return;

    // no se puede poner segunda bomba sobre la misma casilla.
    for (Bomb* active : activatedBombs)
        if (mMaze->getCasillaAtPosition(active->getPosition()) == block)
            return;

    if (!mHero->canSpawnBomb()) return;

    Bomb* bomb = nullptr;
    if (!notActivatedBombs.empty())
    {
        bomb = notActivatedBombs.front();
        notActivatedBombs.pop_front();
    }
    else
    {
        bomb = new Bomb(Vector3::ZERO,
            mSM->getRootSceneNode()->createChildSceneNode(), mSM);
    }

    // La casilla es hija de Maze, asi que getPosition() es local
    Vector3 worldCenter = block->getPosition() + mMaze->getPosition();
    bomb->activate(worldCenter);
    activatedBombs.push_back(bomb);
}

bool IG2Project::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (evt.keysym.sym == SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
        return true;
    }

    if (!in_game)
    {
        if (mHero != nullptr && evt.keysym.sym == SDLK_s)
        {
            in_game = true;
            setup_game_scene();
        }
        return true;
    }

    if (mHero == nullptr) return true;

    if (evt.keysym.sym == SDLK_k)
        cout << "Position of Sinbad: " << mHero->getPosition() << endl;
    else if (evt.keysym.sym == SDLK_UP)
        mHero->setDirection(Vector3::NEGATIVE_UNIT_Z);
    else if (evt.keysym.sym == SDLK_DOWN)
        mHero->setDirection(Vector3::UNIT_Z);
    else if (evt.keysym.sym == SDLK_LEFT)
        mHero->setDirection(Vector3::NEGATIVE_UNIT_X);
    else if (evt.keysym.sym == SDLK_RIGHT)
        mHero->setDirection(Vector3::UNIT_X);
    else if (evt.keysym.sym == SDLK_SPACE)
        spawnBomb();

    return true;
}

void IG2Project::shutdown() {

    for (Bomb* bomb : activatedBombs) delete bomb;
    for (Bomb* bomb : notActivatedBombs) delete bomb;
    activatedBombs.clear();
    notActivatedBombs.clear();

    delete mMaze;
    mShaderGenerator->removeSceneManager(mSM);
    mSM->removeRenderQueueListener(mOverlaySystem);

    mRoot->destroySceneManager(mSM);

    delete mTrayMgr;  mTrayMgr = nullptr;
    delete mCamMgr; mCamMgr = nullptr;

    // do not forget to call the base 
    OgreBites::ApplicationContext::shutdown();
}

void IG2Project::setup(void) {

    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();

    // Create the scene manager
    mSM = mRoot->createSceneManager();

    // Register our scene with the RTSS
    mShaderGenerator->addSceneManager(mSM);
        
    mSM->addRenderQueueListener(mOverlaySystem);
    //mTrayMgr = new OgreBites::TrayManager("TrayGUISystem", mWindow.render);
    //mTrayMgr = new OgreBites::TrayManager("TrayGUISystem", getRenderWindow());
    //mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    //addInputListener(mTrayMgr);

    // Adds the listener for this object
    addInputListener(this);
    setupScene();
}

void IG2Project::setupScene(void) {

    //------------------------------------------------------------------------
    // Creating the camera

    Camera* cam = mSM->createCamera("Cam");
    cam->setNearClipDistance(1);
    cam->setFarClipDistance(10000);
    cam->setAutoAspectRatio(true);
    //cam->setPolygonMode(Ogre::PM_WIREFRAME);

    mCamNode = mSM->getRootSceneNode()->createChildSceneNode("nCam");
    mCamNode->attachObject(cam);

    //mCamNode->setPosition(0, -1000, 200);
    //mCamNode->lookAt(Ogre::Vector3(0, 1, -1.5), Ogre::Node::TS_WORLD);
    
    mCamNode->setPosition(0, 0, 350);
    mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

    // and tell it to render into the main window
    Viewport* vp = getRenderWindow()->addViewport(cam);

    mCamMgr = new OgreBites::CameraMan(mCamNode);
    addInputListener(mCamMgr);
    mCamMgr->setStyle(OgreBites::CS_ORBIT);


    mHero = new Hero(Vector3(0,0,0), mSM->getRootSceneNode()->createChildSceneNode(), mSM, "Sinbad.mesh", 10.0f);
    mHero->setScale(Vector3(10,10,10));

    //mBomb = new Bomb(Vector3(0,0,0), mSM->getRootSceneNode()->createChildSceneNode(),mSM);


    vp = getRenderWindow()->getViewport(0);
    vp->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

    Ogre::Light* directionalLight1 = mSM->createLight("DirectionalLight1");
    directionalLight1->setType(Light::LT_DIRECTIONAL);
    directionalLight1->setDiffuseColour(1.0f, 1.0f, 1.0f);

    // Node with the light attached
    Ogre::SceneNode* nodeDir = mSM->getRootSceneNode()->createChildSceneNode();
    nodeDir->setDirection(Ogre::Vector3(0, -1, -1));
    nodeDir->attachObject(directionalLight1);

    //Light* luz = mSM->createLight("Luz_inicio");
    //Ogre::Light::LightTypes heroLightType = Light::LightTypes::LT_DIRECTIONAL;
    //luz->setType(heroLightType);

    //mHeroLightNode = mSM->getRootSceneNode()->createChildSceneNode("node_luz");
    //mHeroLightNode->setPosition(Vector3(0, 100, 0));
    //mHeroLightNode->attachObject(luz);
    //mHeroLightNode->setDirection(Vector3(0, 0, 0));

    
    floorNode = mSM->getRootSceneNode()->createChildSceneNode();
    Ogre::MeshPtr floor_mesh = Ogre::MeshManager::getSingleton().createPlane("floor_inicio", /*Nombre de la malla creada*/
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, /*El grupo que pertenece este "recurso"*/
        Plane(Vector3::UNIT_Y, 0), /*El plano y su normal*/
        300, 150, /*Ancho y alto*/
        50, 50, /*El numero de segmentos en x e y */
        true, /*Habilitar la normal normal*/
        1, /*wtf el numero de las coordenadas de textura*/
        1, 1, /*Los tiles en el que se divide la malla en x e y*/
        Vector3::UNIT_Z);

    Ogre::Entity* backgroundEntity = mSM->createEntity(floor_mesh);
    floorNode->attachObject(backgroundEntity);
    backgroundEntity->setMaterialName("stoneFloorInitial");
    floorNode->translate(Vector3(0, -50, 0));
    //if (heroLightType == Ogre::Light::LT_DIRECTIONAL) {

    //}
    //else {
    //    mHeroLightNode = mHero->createChildSceneNode();
    //    mHeroLightNode->attachObject(luz);

    //    if (heroLightType == Ogre::Light::LT_POINT) {
    //        luz->setAttenuation(500, 1, 0.0001f, 0.0001f);
    //        luz->setDiffuseColour(1, 1, 1);
    //        mHeroLightNode->setPosition(Vector3(0,30,0));
    //    }
    //    else if (heroLightType == Ogre::Light::LT_SPOTLIGHT) 
    //    {
    //        mHeroLightNode->setDirection(Vector3(0,-1,0));
    //        luz->setSpotlightRange(Degree(45.0f),Degree(90.0f),2);
    //        luz->setDiffuseColour(1,1,1);
    //        mHeroLightNode->setPosition(Vector3(0, 60, 0));
    //    }
    //    else throw 1;
    //}


    //mTrayMgr->createLabel(OgreBites::TL_BOTTOMRIGHT, "Text_Informacion","Informacion",30);
    //mTextbox = mTrayMgr->createTextBox(OgreBites::TL_BOTTOMRIGHT, "TextBox_Vida", "", 200, 100);
    //mTextbox->setText("Vida: \nPuntos: ");
    //mTextbox->setScrollPercentage(0);
}


void IG2Project::setup_game_scene()
{
    mCamNode->setPosition(0, 1000, 200);
    mCamNode->lookAt(Ogre::Vector3(0, 1, -1.5), Ogre::Node::TS_WORLD);

    mMaze = new Maze(Vector3{0,0,0},mSM->getRootSceneNode()->createChildSceneNode(),mSM,"cube.mesh"/*posicion,scenenode,scenemanager,la malla*/);
    mMaze->createMaze("map.txt");

    //mHero = new Hero(mMaze->getHeroStartPosition(), mSM->getRootSceneNode()->createChildSceneNode(), mSM, "Sinbad.mesh",10.0f);
    mHero->setPosition(mMaze->getHeroStartPosition());
    mHero->setScale(Vector3(1, 1, 1));
    mHero->setup_ingame();
    mVillano = new Villano(mMaze->getVillanoStartPosition(), mSM->getRootSceneNode()->createChildSceneNode(), mSM, 10.0f);
    villanoAlive = true;

    Viewport* vp = getRenderWindow()->getViewport(0);

    vp = getRenderWindow()->getViewport(0);
    vp->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

    mSM->destroyAllLights();
    Light* luz = mSM->createLight("Luz_ingame");
    Ogre::Light::LightTypes heroLightType = mMaze->getHeroLightType();
    luz->setType(heroLightType);

    if (heroLightType == Ogre::Light::LT_DIRECTIONAL) {
        mHeroLightNode = mSM->getRootSceneNode()->createChildSceneNode("node_luz");
        mHeroLightNode->attachObject(luz);
        mHeroLightNode->setDirection(Vector3(-1, -1, -1));
    }
    else {
        mHeroLightNode = mHero->createChildSceneNode();
        mHeroLightNode->attachObject(luz);

        if (heroLightType == Ogre::Light::LT_POINT) {
            luz->setAttenuation(500, 1, 0.0001f, 0.0001f);
            luz->setDiffuseColour(1, 1, 1);
            mHeroLightNode->setPosition(Vector3(0, 30, 0));
        }
        else if (heroLightType == Ogre::Light::LT_SPOTLIGHT)
        {
            mHeroLightNode->setDirection(Vector3(0, -1, 0));
            luz->setSpotlightRange(Degree(45.0f), Degree(90.0f), 2);
            luz->setDiffuseColour(1, 1, 1);
            mHeroLightNode->setPosition(Vector3(0, 60, 0));
        }
        else throw 1;
    }

    //mTrayMgr = new OgreBites::TrayManager("TrayGUISystem", mWindow.render);
    mTrayMgr = new OgreBites::TrayManager("TrayGUISystem", getRenderWindow());
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    addInputListener(mTrayMgr);

    mTrayMgr->createLabel(OgreBites::TL_BOTTOMRIGHT, "Text_Informacion", "Informacion", 30);
    mTextbox = mTrayMgr->createTextBox(OgreBites::TL_BOTTOMRIGHT, "TextBox_Vida", "", 200, 100);
    mTextbox->setText("Vida: \nPuntos: ");
    mTextbox->setScrollPercentage(0);
}