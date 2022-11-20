#ifndef AUDIOLISTENER_H
#define AUDIOLISTENER_H

#ifdef _WIN32
#include <al.h>
#include <alc.h>
#endif
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#include "component.h"

class AudioListener : public Component
{
public:
    AudioListener();
    GameObject* mOwner;

    Vec3 pos = Vec3 (0.0f,0.0f,0.0f);
    Vec3 vel = Vec3 (0.0f,0.0f,0.0f);
    Vec3 dir = Vec3 (0.0f,0.0f,1.0f);
    Vec3 up = Vec3 (0.0f,1.0f,0.0f);




};

#endif // AUDIOLISTENER_H
