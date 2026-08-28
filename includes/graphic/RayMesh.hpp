/**
 * @file RayMesh.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYMESH_HPP_
#define RAYMESH_HPP_

//Raylib (without conflict)
#include "RaylibAliases.hpp"

//Interface
#include "IMesh.hpp"

/**
 * @brief Raylib Mesh class - owns the loaded geometry (raylib's own Model
 *        struct is the mesh+material data, not positioned). Many RayModel
 *        instances can reference one RayMesh; deleting a model never
 *        touches the mesh.
 */
class RayMesh : public graphic::IMesh {

    public:
        RayMesh(std::string path) {
            _model = raylib::LoadModel(path.c_str());
            _boundingBox = raylib::GetModelBoundingBox(_model);
        }

        ~RayMesh() {
            raylib::UnloadModel(_model);
        }

        bool isReady() const override {
            return _model.meshCount > 0;
        }

        raylib::Model &handle() {
            return _model;
        }

        raylib::BoundingBox boundingBox() const {
            return _boundingBox;
        }

    private:
        raylib::Model _model;
        raylib::BoundingBox _boundingBox;
};

/** @} */

#endif /* !RAYMESH_HPP_ */
