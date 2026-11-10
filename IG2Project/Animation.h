#pragma once
#include <OgreAnimation.h>
#include <OgreKeyFrame.h>

struct NodeAnimation
{
    std::string name;
    Ogre::AnimationState* state;
    Ogre::Animation* animation;
    Ogre::NodeAnimationTrack* track;
    std::vector<Ogre::TransformKeyFrame*> keyframes;

    void setup(Ogre::SceneManager* sm, std::string name, float duration)
    {
        animation = sm->createAnimation(name, duration);
        track = animation->createNodeTrack(0);
        state = sm->createAnimationState(name);
        ani_duration = duration;
    }

    void attachTo(Ogre::Node* node) 
    {
        track->setAssociatedNode(node);
    }

    void addKeyframe(Real timepos,Vector3 position, Quaternion rotation, Vector3 scale) 
    {
        Ogre::TransformKeyFrame* frame = track->createNodeKeyFrame(timepos);
        frame->setTranslate(position);
        frame->setRotation(rotation);
        frame->setScale(scale);
        keyframes.push_back(frame);
    }

    float ani_duration = 0;
    float ani_time_elapsed = 0;
};

