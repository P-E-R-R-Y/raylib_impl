/**
 * @file RayAudioModule.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYAUDIO_MODULE_HPP
#define RAYAUDIO_MODULE_HPP

#include "IAudioModule.hpp"

#include "audio/RayMusic.hpp"
#include "audio/RaySound.hpp"
#include "audio/RaySoundBuffer.hpp"

//Raylib (without conflict)
#include "RaylibAliases.hpp"

/**
 * @brief L'ouverture du peripherique audio de raylib, comptee par reference.
 *
 * Elle est independante de InitWindow, donc elle vit ici plutot que dans le
 * module graphique.
 *
 * Rien n'est ouvert dans le constructeur : celui-ci s'execute au dlopen,
 * bien avant qu'on sache si ce vendor sera choisi. Ouvrir le peripherique a
 * ce moment le confisquerait a un vendor concurrent que la borne serait en
 * train de preferer - et le journal de raylib s'afficherait au demarrage
 * pour une bibliotheque que personne n'a demandee.
 *
 * Le compte evite aussi qu'un vingtieme son reouvre le peripherique, et
 * fait qu'il ne se ferme que lorsque plus rien ne joue.
 */
class RayAudioModule : public IAudioModule {

public:
    RayAudioModule() = default;
    ~RayAudioModule() { stop(); }

    const char *type() const override { return IAudioModule::contract; }
    const char *name() const override { return "raylib"; }

    // music
    audio::IMusic *createMusic(std::string path) override {
        if (!start())
            return nullptr;
        _opened++;
        return new RayMusic(path);
    }
    void deleteMusic(audio::IMusic *music) override { drop(music); }

    // sound buffer
    audio::ISoundBuffer *createSoundBuffer(std::string path) override {
        if (!start())
            return nullptr;
        _opened++;
        return new RaySoundBuffer(path);
    }
    void deleteSoundBuffer(audio::ISoundBuffer *buffer) override { drop(buffer); }

    // sound
    audio::ISound *createSound(audio::ISoundBuffer *buffer) override {
        if (!buffer || !start())
            return nullptr;
        _opened++;
        return new RaySound(*static_cast<RaySoundBuffer *>(buffer));
    }
    void deleteSound(audio::ISound *sound) override { drop(sound); }

private:
    /** @brief Ouvre le peripherique au premier son demande, et une seule fois. */
    bool start() {
        if (_started)
            return true;
        raylib::InitAudioDevice();
        /* On demande a raylib plutot que de croire l'appel : sans carte son
         * disponible, InitAudioDevice ne dit rien mais ne prepare rien, et
         * un LoadSound derriere travaillerait dans le vide. */
        if (!raylib::IsAudioDeviceReady())
            return false;
        _started = true;
        return true;
    }

    void stop() {
        if (!_started)
            return;
        raylib::CloseAudioDevice();
        _started = false;
    }

    /** @brief Detruit, et rend le peripherique quand plus rien ne joue. */
    template <typename T>
    void drop(T *object) {
        if (!object)
            return;
        delete object;
        if (_opened > 0)
            _opened--;
        if (_opened == 0)
            stop();
    }

    unsigned _opened = 0;
    bool _started = false;
};

/** @} */

#endif /* !RAYAUDIO_MODULE_HPP */
