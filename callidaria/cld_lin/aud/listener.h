#pragma once

#include <iostream>
#include <glm/glm.hpp>

class Listener
{
public:
    Listener() { Listener(glm::vec3(0,0,0),glm::vec3(0,0,0),glm::vec3(0,0,-1)); }
    Listener(glm::vec3 pos,glm::vec3 vel,glm::vec3 ori) {
        alListener3f(AL_POSITION,pos.x,pos.y,pos.z);
        alListener3f(AL_VELOCITY,vel.x,vel.y,vel.z);
        alListener3f(AL_ORIENTATION,ori.x,ori.y,ori.z);
    }
};
