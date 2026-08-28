/**
 * @file RayGamepad.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYGAMEPAD_HPP_
#define RAYGAMEPAD_HPP_

//Raylib (without conflict)
#include "RaylibAliases.hpp"

//Interface
#include "IGamepad.hpp"

#include <unordered_map>

class RayGamepad : public graphic::IGamepad {

    public:
        RayGamepad(int index = 0) : _index(index) {}

        ~RayGamepad() override = default;

        bool isAvailable() const override {
            return raylib::IsGamepadAvailable(_index);
        }

        bool isButtonPressed(Button button) const override {
            return raylib::IsGamepadButtonPressed(_index, _buttons.at(button));
        }
        bool isButtonDown(Button button) const override {
            return raylib::IsGamepadButtonDown(_index, _buttons.at(button));
        }
        bool isButtonReleased(Button button) const override {
            return raylib::IsGamepadButtonReleased(_index, _buttons.at(button));
        }
        bool isButtonUp(Button button) const override {
            return raylib::IsGamepadButtonUp(_index, _buttons.at(button));
        }

        float getAxisMovement(Axis axis) const override {
            return raylib::GetGamepadAxisMovement(_index, _axes.at(axis));
        }

    private:
        int _index;

        const std::unordered_map<IGamepad::Button, int> _buttons = {
            {IGamepad::BUTTON_A, raylib::GAMEPAD_BUTTON_RIGHT_FACE_DOWN},
            {IGamepad::BUTTON_B, raylib::GAMEPAD_BUTTON_RIGHT_FACE_RIGHT},
            {IGamepad::BUTTON_X, raylib::GAMEPAD_BUTTON_RIGHT_FACE_LEFT},
            {IGamepad::BUTTON_Y, raylib::GAMEPAD_BUTTON_RIGHT_FACE_UP},
            {IGamepad::BUTTON_LEFT_BUMPER, raylib::GAMEPAD_BUTTON_LEFT_TRIGGER_1},
            {IGamepad::BUTTON_RIGHT_BUMPER, raylib::GAMEPAD_BUTTON_RIGHT_TRIGGER_1},
            {IGamepad::BUTTON_BACK, raylib::GAMEPAD_BUTTON_MIDDLE_LEFT},
            {IGamepad::BUTTON_START, raylib::GAMEPAD_BUTTON_MIDDLE_RIGHT},
            {IGamepad::BUTTON_LEFT_THUMB, raylib::GAMEPAD_BUTTON_LEFT_THUMB},
            {IGamepad::BUTTON_RIGHT_THUMB, raylib::GAMEPAD_BUTTON_RIGHT_THUMB},
            {IGamepad::BUTTON_DPAD_UP, raylib::GAMEPAD_BUTTON_LEFT_FACE_UP},
            {IGamepad::BUTTON_DPAD_RIGHT, raylib::GAMEPAD_BUTTON_LEFT_FACE_RIGHT},
            {IGamepad::BUTTON_DPAD_DOWN, raylib::GAMEPAD_BUTTON_LEFT_FACE_DOWN},
            {IGamepad::BUTTON_DPAD_LEFT, raylib::GAMEPAD_BUTTON_LEFT_FACE_LEFT},
        };

        const std::unordered_map<IGamepad::Axis, int> _axes = {
            {IGamepad::AXIS_LEFT_X, raylib::GAMEPAD_AXIS_LEFT_X},
            {IGamepad::AXIS_LEFT_Y, raylib::GAMEPAD_AXIS_LEFT_Y},
            {IGamepad::AXIS_RIGHT_X, raylib::GAMEPAD_AXIS_RIGHT_X},
            {IGamepad::AXIS_RIGHT_Y, raylib::GAMEPAD_AXIS_RIGHT_Y},
            {IGamepad::AXIS_LEFT_TRIGGER, raylib::GAMEPAD_AXIS_LEFT_TRIGGER},
            {IGamepad::AXIS_RIGHT_TRIGGER, raylib::GAMEPAD_AXIS_RIGHT_TRIGGER},
        };
};

/** @} */

#endif /* !RAYGAMEPAD_HPP_ */
