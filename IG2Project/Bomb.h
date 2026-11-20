#pragma once

#include "IG2Object.h"
#include "Animation.h"
#include <OgreParticleSystem.h>

// Configuracion de la bomba (segundos).
constexpr float MAX_SCALE = 0.06F;
constexpr float MIN_SCALE = 0.05F;
constexpr float EXPLOSTION_TIME = 4.0F;

class Bomb : public IG2Object
{
public:
    Bomb(Vector3 initPos, SceneNode* node, SceneManager* sceneMng);

    void activate(const Vector3& position);
    void update(float dt);
    void toggle(); // Se mantiene por compatibilidad con las pruebas anteriores.

    bool isExploded() const;
    bool isActive() const;

private:
    Ogre::SceneNode* cuerpo = nullptr;
    Ogre::SceneNode* mecha = nullptr;
    Ogre::SceneNode* particleSystemNode = nullptr;
    Ogre::ParticleSystem* particleSystem = nullptr;

    bool is_active = false;
    bool exploded = false;
    float elapsed_time = 0.0f;

    NodeAnimation cuerpoAnimation;
    NodeAnimation mechaAnimation;
};
