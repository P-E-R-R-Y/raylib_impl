/**
 * @file RayMusic.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYMUSIC_HPP_
#define RAYMUSIC_HPP_

//Raylib (without conflict)
#include "RaylibAliases.hpp"

//Interface
#include "IMusic.hpp"

/**
 * @brief raylib music stream.
 *
 * Two departures from raylib :
 * - the three-state machine (stopped/playing/paused) is tracked here,
 *   raylib being unable to tell "paused" from "stopped" ;
 * - setTime() clamps the position, SeekMusicStream() wrapping past the end.
 */
class RayMusic : public audio::IMusic {

    public:
        RayMusic(std::string path) {
            _music = raylib::LoadMusicStream(path.c_str());
            _state = STOPPED;
            _loop = false;
            _volume = 1.0f;
        }

        ~RayMusic() override {
            raylib::UnloadMusicStream(_music);
        }

        bool isReady() const override {
            return raylib::IsMusicValid(_music);
        }

        // play/pause/stop do ONLY what they say : from PLAYING, play() has
        // nothing to do. raylib has no readable "paused" state
        // (IsMusicStreamPlaying is false there just as when stopped), so we
        // track it ourselves.
        void play() override {
            if (_state == PLAYING)
                return;
            if (_state == PAUSED)
                raylib::ResumeMusicStream(_music);
            else
                raylib::PlayMusicStream(_music);
            _state = PLAYING;
        }

        void pause() override {
            if (_state != PLAYING)
                return;
            raylib::PauseMusicStream(_music);
            _state = PAUSED;
        }

        void stop() override {
            if (_state == STOPPED)
                return;
            raylib::StopMusicStream(_music);
            _state = STOPPED;
        }

        void update() override {
            raylib::UpdateMusicStream(_music);

            // raylib loops on its own : without loop it is up to us to stop
            // at the end, which leaves the state of a stop()
            if (!_loop && _state == PLAYING &&
                raylib::GetMusicTimePlayed(_music) >= raylib::GetMusicTimeLength(_music)) {
                this->stop();
            }
        }

        void setVolume(float volume) override {
            _volume = volume;
            raylib::SetMusicVolume(_music, volume);
        }

        float getVolume() const override {
            return _volume;
        }

        void setLoop(bool loop) override {
            _loop = loop;
        }

        bool getLoop() const override {
            return _loop;
        }

        void setTime(float position) override {
            // raylib hands the raw position to dr_wav, which wraps past the
            // end : clamp to honour the contract
            raylib::SeekMusicStream(_music, clampTime(position));
        }

        float getTime() const override {
            return raylib::GetMusicTimePlayed(_music);
        }

        float getLength() const override {
            return raylib::GetMusicTimeLength(_music);
        }

        //raylib has no positional audio for music streams either : same
        //graceful no-op as RaySound
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
        float clampTime(float position) const {
            const float length = getLength();

            if (position < 0.f) return 0.f;
            return (position > length) ? length : position;
        }

        enum State { STOPPED, PLAYING, PAUSED };

        raylib::Music _music;
        State _state;
        bool _loop;
        float _volume;
        Vector3f _position{};
        Vector3f _velocity{};
};

/** @} */

#endif /* !RAYMUSIC_HPP_ */
