/**
 * @file RaySound.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYSOUND_HPP_
#define RAYSOUND_HPP_

//Raylib (without conflict)
#include "RaylibAliases.hpp"

//Interface
#include "ISound.hpp"

//encapsulation
#include "RaySoundBuffer.hpp"

/**
 * @brief Raylib Sound class - a LoadSoundAlias() of a RaySoundBuffer, does
 *        not own the buffer. Deleting the sound never touches the buffer.
 *
 * play() restarts the sound, where raylib would overlap playbacks by
 * default. This is the only deliberate divergence from IMusic::play() : an
 * ISound is a one-shot, not a transport.
 */
class RaySound : public audio::ISound {

    public:
        RaySound(RaySoundBuffer &buffer) : _buffer(buffer) {
            _alias = raylib::LoadSoundAlias(buffer.handle());
            _volume = 1.0f;
        }

        ~RaySound() override {
            raylib::UnloadSoundAlias(_alias);
        }

        bool isReady() const override {
            return raylib::IsSoundValid(_alias);
        }

        void play() override {
            //restart rather than overlap : matches sf::Sound's native
            //behaviour instead of raylib's default auto-pooling, so the
            //contract's play() semantics are the same across vendors
            raylib::StopSound(_alias);
            raylib::PlaySound(_alias);
        }

        void pause() override {
            raylib::PauseSound(_alias);
        }

        void stop() override {
            raylib::StopSound(_alias);
        }

        void setVolume(float volume) override {
            _volume = volume;
            raylib::SetSoundVolume(_alias, volume);
        }

        float getVolume() const override {
            return _volume;
        }

        //raylib has no positional audio in its core Sound API : store and
        //keep playing at full volume, a graceful no-op, not a broken one
        void setPosition(Vector3f position) override {
            _position = position;
        }
        Vector3f getPosition() const override {
            return _position;
        }

        void setVelocity(Vector3f velocity) override {
            _velocity = velocity;
        }
        Vector3f getVelocity() const override {
            return _velocity;
        }

    private:
        RaySoundBuffer &_buffer;
        raylib::Sound _alias;
        float _volume;
        Vector3f _position{};
        Vector3f _velocity{};
};

/** @} */

#endif /* !RAYSOUND_HPP_ */
