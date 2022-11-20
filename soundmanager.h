#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include "audiosource.h"
#include "audiolistener.h"

class SoundManager
{
public:
    static SoundManager* getInstance()          ///< Get pointer to singleton instance.
    {
        if (!mInstance)
            mInstance = new SoundManager();
        return mInstance;
    }
    bool init();
    void cleanUp();
    static AudioSource* createSource(GameObject* owner, std::string name, Vec3 pos, std::string filePath = "", bool loop = false, float gain = 1.0);
    void updateListener(Vec3 pos, Vec3 vel, Vec3 dir, Vec3 up);
private:
    SoundManager();                         ///< Private constructor.
    SoundManager(SoundManager const&);      ///< Private copy constructor.
    void operator=(SoundManager const&);    ///< Private class assignment operator.
    static SoundManager* mInstance;         ///< Singleton instance pointer.
    /// Debug function.
    /**
        Checks for and outputs OpenAL errors.
    **/
    bool checkError();

    ALCdevice* mDevice;                 ///< Pointer to the ALC Device.
    ALCcontext* mContext;               ///< Pointer to the ALC Context.

    AudioListener mListener;               ///< The listener data structure. (Temporary)

};

#endif // SOUNDMANAGER_H
