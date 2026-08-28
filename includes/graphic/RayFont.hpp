/**
 * @file RayFont.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYFONT_HPP_
#define RAYFONT_HPP_

//Raylib (without conflict)
#include "RaylibAliases.hpp"

//Interface
#include "IFont.hpp"

/**
 * @brief Raylib Font class - owns the loaded font, independent of any
 *        RayText built from it.
 */
class RayFont : public graphic::IFont {

    public:
        RayFont(std::string path) {
            _font = raylib::LoadFont(path.c_str());
        }

        ~RayFont() {
            raylib::UnloadFont(_font);
        }

        bool isReady() const override {
            return raylib::IsFontValid(_font);
        }

        raylib::Font handle() const {
            return _font;
        }

    private:
        raylib::Font _font;
};

/** @} */

#endif /* !RAYFONT_HPP_ */
