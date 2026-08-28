/**
 * @file RaySoundBuffer.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYSOUNDBUFFER_HPP_
#define RAYSOUNDBUFFER_HPP_

//Raylib (without conflict)
#include "RaylibAliases.hpp"

//Interface
#include "ISoundBuffer.hpp"

/**
 * @brief Raylib SoundBuffer class - owns the loaded samples (raylib's own
 *        LoadSound() result). RaySound instances are LoadSoundAlias()'d
 *        from this and don't own the samples themselves.
 */
class RaySoundBuffer : public audio::ISoundBuffer {

    public:
        RaySoundBuffer(std::string path) {
            _sound = raylib::LoadSound(path.c_str());
        }

        ~RaySoundBuffer() {
            raylib::UnloadSound(_sound);
        }

        bool isReady() const override {
            return raylib::IsSoundValid(_sound);
        }

        float getLength() const override {
            return _sound.frameCount / static_cast<float>(_sound.stream.sampleRate);
        }

        raylib::Sound handle() const {
            return _sound;
        }

    private:
        raylib::Sound _sound;
};

/** @} */

#endif /* !RAYSOUNDBUFFER_HPP_ */
