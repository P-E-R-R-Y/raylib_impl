/**
 * @file RayText.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYTEXT_HPP_
#define RAYTEXT_HPP_

//standard
#include <string>

//Raylib (without conflict)
#include "RaylibAliases.hpp"

//Interface
#include "IText.hpp"

//encapsulation
#include "RayFont.hpp"

/**
 * @brief Raylib Text class - references a RayFont, does not own it.
 *        Deleting the text never touches the font.
 */
class RayText : public graphic::IText {

    public:
        RayText(std::string data, RayFont &font) : _font(&font) {
            _data = data;
            _position = {0, 0};
            _color = raylib::Color{255, 255, 255, 255};
            _size = 20;
            _spacing = 1;
            _rotation = 0;
        }

        ~RayText() override = default;

        bool isReady() const override {
            return _font->isReady();
        }

        void setText(const std::string text) override {
            _data = text;
        }

        std::string getText() const override {
            return _data;
        }

        void setFont(graphic::IFont *font) override {
            _font = static_cast<RayFont *>(font);
        }

        void setFontSize(unsigned int size) override {
            _size = size;
        }
        unsigned int getFontSize() const override {
            return _size;
        }

        void setTextColor(Color color) override {
            _color = raylib::Color{static_cast<unsigned char>(color.r), static_cast<unsigned char>(color.g),
                                    static_cast<unsigned char>(color.b), static_cast<unsigned char>(color.a)};
        }

        Color getTextColor() const override {
            return Color{_color.r, _color.g, _color.b, _color.a};
        }

        void setPosition(Vector2f position) override {
            _position = raylib::Vector2{static_cast<float>(position.x), static_cast<float>(position.y)};
        }
        Vector2f getPosition() const override {
            return Vector2f{_position.x, _position.y};
        }

        void setRotation(float angle) override {
            _rotation = angle;
        }

        float getRotation() const override {
            return _rotation;
        }

        friend class RayWindow;

    private:
        std::string _data;
        raylib::Vector2 _position;
        raylib::Color _color;
        RayFont *_font;
        unsigned int _size;
        float _rotation;
        float _spacing;
};

/** @} */

#endif /* !RAYTEXT_HPP_ */
