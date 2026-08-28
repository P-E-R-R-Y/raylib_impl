/**
 * @file RayTexture.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYTEXTURE_HPP_
#define RAYTEXTURE_HPP_

//Raylib (without conflict)
#include "RaylibAliases.hpp"

//Interface
#include "ITexture.hpp"

/**
 * @brief Raylib Texture class - owns the GPU texture. RaySprite/RayModel
 *        only reference it, never own it: this survives as long as the
 *        caller keeps it alive, regardless of how many sprites/models get
 *        created and destroyed against it.
 */
class RayTexture : public graphic::ITexture {

    public:
        RayTexture(std::string path) {
            _texture = raylib::LoadTexture(path.c_str());
        }

        ~RayTexture() {
            raylib::UnloadTexture(_texture);
        }

        bool isReady() const override {
            return raylib::IsTextureValid(_texture);
        }

        Vector2f getSize() const override {
            return {static_cast<float>(_texture.width), static_cast<float>(_texture.height)};
        }

        raylib::Texture2D handle() const {
            return _texture;
        }

    private:
        raylib::Texture2D _texture;
};

/** @} */

#endif /* !RAYTEXTURE_HPP_ */
