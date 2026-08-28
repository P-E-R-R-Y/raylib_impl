/**
 * @file RayCamera.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYCAMERA_HPP_
#define RAYCAMERA_HPP_

//Raylib (without conflict)
#include "RaylibAliases.hpp"

//Interface
#include "ICamera.hpp"

/**
 * @brief Raylib Camera class
 */
class RayCamera : public graphic::ICamera {

    public:
        RayCamera(Vector3f pos = {100.0f, 100.0f, 100.0f}, Vector3f target = {0.f, 0.f, 0.f}, float fov = 45.f) {
            _camera = {0};
            _camera.position = (raylib::Vector3){(float)pos.x, (float)pos.y, (float)pos.z};
            _camera.target = (raylib::Vector3){(float)target.x, (float)target.y, (float)target.z};
            _camera.up = (raylib::Vector3){0.f, 1.f, 0.0f};
            _camera.fovy = fov;
            _camera.projection = raylib::CAMERA_PERSPECTIVE;
            _mode = raylib::CAMERA_CUSTOM;
        }

        ~RayCamera() = default;

        float getFov() const override {
            return _camera.fovy;
        }

        void setFov(float fov) override {
            _camera.fovy = fov;
        }

        ICamera::Mode getMode() const override {
            switch (_mode) {
                case raylib::CameraMode::CAMERA_FREE: return ICamera::Mode::FREE;
                case raylib::CameraMode::CAMERA_ORBITAL: return ICamera::Mode::ORBITAL;
                case raylib::CameraMode::CAMERA_FIRST_PERSON: return ICamera::Mode::FIRST_PERSON;
                case raylib::CameraMode::CAMERA_THIRD_PERSON: return ICamera::Mode::THIRD_PERSON;
                default: return ICamera::Mode::CUSTOM;
            }
        }

        void setMode(ICamera::Mode mode) override {
            switch (mode) {
                case ICamera::Mode::FREE: _mode = raylib::CameraMode::CAMERA_FREE; break;
                case ICamera::Mode::ORBITAL: _mode = raylib::CameraMode::CAMERA_ORBITAL; break;
                case ICamera::Mode::FIRST_PERSON: _mode = raylib::CameraMode::CAMERA_FIRST_PERSON; break;
                case ICamera::Mode::THIRD_PERSON: _mode = raylib::CameraMode::CAMERA_THIRD_PERSON; break;
                default: _mode = raylib::CameraMode::CAMERA_CUSTOM; break;
            }
        }

        Vector3f getPosition() const override {
            return {_camera.position.x, _camera.position.y, _camera.position.z};
        }

        void setPosition(Vector3f position) override {
            _camera.position = {float(position.x), float(position.y), float(position.z)};
        }

        ICamera::Projection getProjection() const override {
            return _camera.projection == raylib::CAMERA_ORTHOGRAPHIC ? ICamera::ORTHOGRAPHIC : ICamera::PERSPECTIVE;
        }

        void setProjection(ICamera::Projection projection) override {
            _camera.projection = projection == ICamera::ORTHOGRAPHIC ? raylib::CAMERA_ORTHOGRAPHIC : raylib::CAMERA_PERSPECTIVE;
        }

        Vector3f getTarget() const override {
            return {_camera.target.x, _camera.target.y, _camera.target.z};
        }

        void setTarget(Vector3f target) override {
            _camera.target = {float(target.x), float(target.y), float(target.z)};
        }

        Vector3f getUp() const override {
            return {_camera.up.x, _camera.up.y, _camera.up.z};
        }

        void setUp(Vector3f up) override {
            _camera.up = {float(up.x), float(up.y), float(up.z)};
        }

        friend class RayWindow;

    private:
        raylib::Camera _camera;
        raylib::CameraMode _mode;
};

/** @} */

#endif /* !RAYCAMERA_HPP_ */
