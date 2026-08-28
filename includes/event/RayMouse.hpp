/**
 * @file RayMouse.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYMOUSE_HPP_
#define RAYMOUSE_HPP_

//Raylib (without conflict)
#include "RaylibAliases.hpp"

//Interface
#include "IMouse.hpp"

/**
 * @brief raylib mouse.
 *
 * The one place in the wrapper resting on a numeric coincidence :
 * IMouse::Buttons and raylib's MouseButton share the same values, so the
 * value is passed straight through with no mapping table. Reordering
 * either enum breaks it silently.
 */
class RayMouse : public graphic::IMouse {

    public:
        RayMouse() = default;

        ~RayMouse() override = default;

        bool isButtonPressed(Buttons key) const override {
            return raylib::IsMouseButtonPressed(key);
        }
        bool isButtonDown(Buttons key) const override {
            return raylib::IsMouseButtonDown(key);
        }
        bool isButtonReleased(Buttons key) const override {
            return raylib::IsMouseButtonReleased(key);
        }
        bool isButtonUp(Buttons key) const override {
            return raylib::IsMouseButtonUp(key);
        }

        Vector2f getPosition() const override {
            return {float(raylib::GetMouseX()), float(raylib::GetMouseY())};
        }
        void setPosition(Vector2f position) override {
            raylib::SetMousePosition(int(position.x), int(position.y));
        }

        float GetMouseWheelMove() const override {
            return raylib::GetMouseWheelMove();
        }
};

/** @} */

#endif /* !RAYMOUSE_HPP_ */
