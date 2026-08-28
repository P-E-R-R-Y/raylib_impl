/**
 * @file RayModel.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYMODEL_HPP_
#define RAYMODEL_HPP_

//Raylib (without conflict)
#include "RaylibAliases.hpp"
namespace raylib {
    #include "raymath.h"
}

//Interface
#include "IModel.hpp"

//encapsulation
#include "RayAnimationSet.hpp"
#include "RayMesh.hpp"
#include "RayTexture.hpp"

#include <utility>


/**
 * @brief Raylib Model class - references a RayMesh, does not own it.
 *
 * KNOWN NON-CONFORMANCE : raylib::UpdateModelAnimation() writes the pose
 * into the mesh's own vertex buffer. Two RayModel sharing one RayMesh
 * cannot sit on two different animation frames at the same instant - the
 * last updateAnimation() called wins. The contract wants the opposite ;
 * raylib is what falls short. Sharing position/rotation/size works.
 *        Deleting the model never touches the mesh, texture or animation
 *        set it was built from/bound to.
 *
 * Note: raylib::UpdateModelAnimation() writes the pose into the mesh's own
 * vertex buffer, so two RayModel instances sharing one RayMesh cannot be at
 * two different animation frames at the same instant - a real raylib
 * constraint, not something this wrapper works around.
 */
class RayModel : public graphic::IModel {

    public:
        RayModel(RayMesh &mesh) : _mesh(mesh), _position{}, _rotation{}, _scale{1, 1, 1},
                                   _animations(nullptr), _animId(0), _animFrame(0) {
        }

        ~RayModel() = default;

        void setTexture(graphic::ITexture *texture) override {
            auto *rayTexture = static_cast<RayTexture *>(texture);
            raylib::Model &model = _mesh.handle();
            for (int i = 0; i < model.materialCount; i++) {
                raylib::SetMaterialTexture(&model.materials[i], raylib::MATERIAL_MAP_DIFFUSE, rayTexture->handle());
            }
        }

        void setAnimations(graphic::IAnimationSet *animations) override {
            _animations = static_cast<RayAnimationSet *>(animations);
            _animFrame = 0;
            _animId = 0;
        }
        int getAnimationsSize() const override {
            return _animations ? _animations->getSize() : 0;
        }
        void setAnimation(int pos) override {
            _animId = pos;
            // changer de clip repart du debut, sur le clip entier
            _first = 0;
            _last = frameCount() > 0 ? frameCount() - 1 : 0;
            _animFrame = 0;
        }
        int getAnimation() const override {
            return _animId;
        }
        bool updateAnimation() override {
            if (frameCount() == 0)
                return false;

            const bool wrapped = (_animFrame >= _last);

            _animFrame = wrapped ? _first : _animFrame + 1;
            apply();
            return wrapped;
        }

        void setFrameRange(int first, int last) override {
            const int frames = frameCount();
            if (frames == 0)
                return;

            _first = clamp(first, frames);
            _last = clamp(last, frames);
            if (_last < _first)
                std::swap(_first, _last);
            setFrame(_first);
        }

        int getFirstFrame() const override { return _first; }
        int getLastFrame() const override { return _last; }

        void setFrame(int frame) override {
            if (frameCount() == 0)
                return;

            _animFrame = (frame < _first || frame > _last) ? _first : frame;
            apply();
        }

        int getFrame() const override {
            return _animFrame;
        }

        Vector3f getPosition() const override {
            return {_position.x, _position.y, _position.z};
        }

        void setPosition(Vector3f position) override {
            _position = {float(position.x), float(position.y), float(position.z)};
        }

        Vector3f getRotation() const override {
            return {_rotation.x, _rotation.y, _rotation.z};
        }

        void setRotation(Vector3f rotation) override {
            _rotation = {float(rotation.x), float(rotation.y), float(rotation.z)};
            _mesh.handle().transform = raylib::MatrixRotateXYZ(
                (raylib::Vector3){DEG2RAD * _rotation.x, DEG2RAD * _rotation.y, DEG2RAD * _rotation.z});
        }

        Vector3f getSize() const override {
            raylib::BoundingBox box = _mesh.boundingBox();
            raylib::Vector3 s = {box.max.x - box.min.x, box.max.y - box.min.y, box.max.z - box.min.z};
            return {s.x * _scale.x, s.y * _scale.y, s.z * _scale.z};
        }

        void setSize(Vector3f size) override {
            raylib::BoundingBox box = _mesh.boundingBox();
            raylib::Vector3 s = {box.max.x - box.min.x, box.max.y - box.min.y, box.max.z - box.min.z};
            if (s.x == 0 || s.y == 0 || s.z == 0) return;

            _scale.x = size.x / s.x;
            _scale.y = size.y / s.y;
            _scale.z = size.z / s.z;
        }

        friend class RayWindow;

    private:
        static int clamp(int frame, int frames) {
            return frame < 0 ? 0 : (frame >= frames ? frames - 1 : frame);
        }

        void apply() {
            raylib::UpdateModelAnimation(_mesh.handle(), *(_animations->handle() + _animId), _animFrame);
        }

        int frameCount() const {
            if (!_animations || _animId < 0 || _animId >= _animations->getSize())
                return 0;
            return (_animations->handle() + _animId)->frameCount;
        }

        RayMesh &_mesh;
        raylib::Vector3 _rotation;
        raylib::Vector3 _position;
        raylib::Vector3 _scale;
        RayAnimationSet *_animations;
        int _animId;
        int _animFrame;
        int _first = 0;
        int _last = 0;
};

/** @} */

#endif /* !RAYMODEL_HPP_ */
