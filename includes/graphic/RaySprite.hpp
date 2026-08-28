/**
 * @file RaySprite.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYSPRITE_HPP_
#define RAYSPRITE_HPP_

//Raylib (without conflict)
#include "RaylibAliases.hpp"

//Interface
#include "ISprite.hpp"

//encapsulation
#include "RayTexture.hpp"

/**
 * @brief Raylib Sprite class - references a RayTexture, does not own it.
 *        Deleting the sprite never touches the texture.
 */
class RaySprite : public graphic::ISprite {

    public:
        RaySprite(RayTexture &texture) : _texture(texture) {
            _position = {0, 0};
            _scale = {1, 1};
            raylib::Texture2D t = _texture.handle();
            _crop = {0, 0, float(t.width), float(t.height)};
            _rotation = 0;
        }

        ~RaySprite() = default;

        bool isReady() const override {
            return _texture.isReady();
        }

        Rect<float> getBounds() const override {
            return {_position.x, _position.y, _crop.width, _crop.height};
        }

        void setCrop(Rect<float> rect) override {
            _crop = {float(rect.x), float(rect.y), float(rect.w), float(rect.h)};
        }

        Vector2f getPosition() const override {
            return {_position.x, _position.y};
        }
        void setPosition(Vector2f position) override {
            _position = {float(position.x), float(position.y)};
        }

        float getRotation() const override {
            return _rotation;
        }

        void setRotation(float angle, bool isRad = false) override {
            _rotation = isRad ? angle * RAD2DEG : angle;
        }

        Vector2f getSize() const override {
            raylib::Texture2D t = _texture.handle();
            return {
                static_cast<float>(t.width) * _scale.x,
                static_cast<float>(t.height) * _scale.y
            };
        }

        void setSize(Vector2f size) override {
            raylib::Texture2D t = _texture.handle();
            if (t.width == 0 || t.height == 0) return;

            _scale.x = size.x / static_cast<float>(t.width);
            _scale.y = size.y / static_cast<float>(t.height);
        }

        friend class RayWindow;

    private:
        RayTexture &_texture;
        raylib::Vector2 _position;
        raylib::Vector2 _scale;
        raylib::Rectangle _crop;
        float _rotation;
};

/** @} */

#endif /* !RAYSPRITE_HPP_ */
