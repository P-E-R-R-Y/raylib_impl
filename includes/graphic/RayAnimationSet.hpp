/**
 * @file RayAnimationSet.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYANIMATIONSET_HPP_
#define RAYANIMATIONSET_HPP_

//Raylib (without conflict)
#include "RaylibAliases.hpp"

//Interface
#include "IAnimationSet.hpp"

#include <string>

/**
 * @brief Raylib AnimationSet class - owns the loaded clips, independent of
 *        any RayModel bound to it.
 */
class RayAnimationSet : public graphic::IAnimationSet {

    public:
        RayAnimationSet(std::string path) {
            _animations = raylib::LoadModelAnimations(path.c_str(), &_size);
        }

        ~RayAnimationSet() {
            if (_animations)
                raylib::UnloadModelAnimations(_animations, _size);
        }

        bool isReady() const override {
            return _animations != nullptr;
        }

        int getSize() const override {
            return _size;
        }

        std::string getName(int index) const override {
            return valid(index) ? _animations[index].name : std::string();
        }

        int getFrameCount(int index) const override {
            return valid(index) ? _animations[index].frameCount : 0;
        }

        raylib::ModelAnimation *handle() const {
            return _animations;
        }

    private:
        bool valid(int index) const {
            return _animations && index >= 0 && index < _size;
        }

        raylib::ModelAnimation *_animations;
        int _size = 0;
};

/** @} */

#endif /* !RAYANIMATIONSET_HPP_ */
