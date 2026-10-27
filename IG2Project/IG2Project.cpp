#include "IG2Project.h"
#include "Maze.h"
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
    if (collisionCooldown > 0)
        collisionCooldown -= evt.timeSinceLastFrame;

    if (mMaze != nullptr) {

        if (mHero != nullptr && mVillano != nullptr) {

            mMaze->update(evt.timeSinceLastFrame, mHero, mVillano);

            mVillano->update(evt.timeSinceLastFrame);

            // Colision entre heroe y villano
            if (collisionCooldown <= 0 &&
                mHero->getAABB().intersects(mVillano->getAABB())) {

                mHero->loseLife();

                if (mHero->getLives() > 0) {

                    // Volver al inicio del laberinto
                    mHero->setPosition(mMaze->getHeroStartPosition());

                    collisionCooldown = 1.0f;
                }
                else {

                    // Fin del juego
                    getRoot()->queueEndRendering();
                    return;
                }
            }
        }
    }

    if (mTextbox != nullptr)
    {
        showCurrentInfo();
    }
}

bool IG2Project::keyPressed(const OgreBites::KeyboardEvent& evt) {

    // ESC key finished the rendering...
    if (evt.keysym.sym == SDLK_ESCAPE) {
        getRoot()->queueEndRendering();
    }

    // Print the Sinbad's position
    else if (evt.keysym.sym == SDLK_k) {
        cout << "Position of Sinbad: " << mSinbadNode->getPosition() << endl;
    }
    else if (evt.keysym.sym == SDLK_UP) {
        cout << "Pressed UP" << endl;
        nextDir = Vector3::NEGATIVE_UNIT_Z;
    }
    else if (evt.keysym.sym == SDLK_DOWN) {
        cout << "Pressed DOWN" << endl;
        nextDir = Vector3::UNIT_Z;
    }
    else if (evt.keysym.sym == SDLK_LEFT) {
        cout << "Pressed LEFT" << endl;
        nextDir = Vector3::NEGATIVE_UNIT_X;
    }
    else if (evt.keysym.sym == SDLK_RIGHT) {
        cout << "Pressed RIGHT" << endl;
        nextDir = Vector3::UNIT_X;
    }

	mHero->setDirection(nextDir);

    return true;
}

void IG2Project::shutdown() {

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
    mTrayMgr = new OgreBites::TrayManager("TrayGUISystem", getRenderWindow());
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    addInputListener(mTrayMgr);

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

    mCamNode->setPosition(0, -1000, 200);
    mCamNode->lookAt(Ogre::Vector3(0, 1, -1.5), Ogre::Node::TS_WORLD);

    // and tell it to render into the main window
    Viewport* vp = getRenderWindow()->addViewport(cam);

    mCamMgr = new OgreBites::CameraMan(mCamNode);
    addInputListener(mCamMgr);
    mCamMgr->setStyle(OgreBites::CS_ORBIT);


    //------------------------------------------------------------------------
    // Creating the light

    //mSM->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
    
    //Light* luz = mSM->createLight("Luz");
    //luz->setType(Ogre::Light::LT_DIRECTIONAL);
    //luz->setDiffuseColour(0.75, 0.75, 0.75);

    //mLightNode = mSM->getRootSceneNode()->createChildSceneNode("nLuz");
    //mLightNode->attachObject(luz);
    //mLightNode->setDirection(Ogre::Vector3(-1, -1, -1));

    mMaze = new Maze(Vector3{0,0,0},mSM->getRootSceneNode()->createChildSceneNode(),mSM,"cube.mesh"/*posicion,scenenode,scenemanager,la malla*/);
    mMaze->createMaze("map.txt");

    mHero = new Hero(mMaze->getHeroStartPosition(), mSM->getRootSceneNode()->createChildSceneNode(), mSM, "Sinbad.mesh",10.0f);
    mVillano = new Villano(mMaze->getVillanoStartPosition(), mSM->getRootSceneNode()->createChildSceneNode(), mSM, 10.0f);


    vp = getRenderWindow()->getViewport(0);
    vp->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

    Light* luz = mSM->createLight("Luz");
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
            mHeroLightNode->setPosition(Vector3(0,30,0));
        }
        else if (heroLightType == Ogre::Light::LT_SPOTLIGHT) 
        {
            mHeroLightNode->setDirection(Vector3(0,-1,0));
            luz->setSpotlightRange(Degree(45.0f),Degree(90.0f),2);
            luz->setDiffuseColour(1,1,1);
            mHeroLightNode->setPosition(Vector3(0, 60, 0));
        }
        else throw 1;
    }


    mTrayMgr->createLabel(OgreBites::TL_BOTTOMRIGHT, "Text_Informacion","Informacion",30);
    mTextbox = mTrayMgr->createTextBox(OgreBites::TL_BOTTOMRIGHT, "TextBox_Vida", "", 200, 100);
    mTextbox->setText("Vida: \nPuntos: ");
    mTextbox->setScrollPercentage(0);
}