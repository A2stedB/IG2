#pragma once
#include <OgreAnimation.h>

struct NodeAnimation
{
    std::string name;
    Ogre::AnimationState* state;
    Ogre::Animation* animation;
    Ogre::NodeAnimationTrack* track;
    std::vector<Ogre::TransformKeyFrame*> keyframes;
};

