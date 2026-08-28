/**
 * @file RayGraphicModule.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYGRAPHIC_MODULE_HPP
#define RAYGRAPHIC_MODULE_HPP

#include "IGraphic3Module.hpp"

#include "window/RayCamera.hpp"
#include "window/RayWindow.hpp"

#include "event/RayGamepad.hpp"
#include "event/RayKeyboard.hpp"
#include "event/RayMouse.hpp"

#include "graphic/RayAnimationSet.hpp"
#include "graphic/RayFont.hpp"
#include "graphic/RayMesh.hpp"
#include "graphic/RayModel.hpp"
#include "graphic/RayPolygon.hpp"
#include "graphic/RaySprite.hpp"
#include "graphic/RayText.hpp"
#include "graphic/RayTexture.hpp"

/**
 * @brief Single concrete class implementing IGraphic3Module, which already
 *        IS-A IGraphic2Module via the interface chain - one createWindow()/
 *        createSprite()/... implementation, exported under two dlsym entry
 *        points (getGraphic2Module/getGraphic3Module) in sources/raylib.cpp,
 *        both returning this same instance.
 */
class RayGraphicModule : public IGraphic3Module {

public:
    RayGraphicModule() = default;
    ~RayGraphicModule() = default;

    const char *type() const override { return IGraphic3Module::contract; }
    const char *name() const override { return "raylib"; }

    // window
    // raylib ne sait pas ouvrir une deuxieme fenetre : nullptr.
    graphic::IWindow3 *createWindow(int32_t screenWidth, int32_t screenHeight, std::string title) override {
        if (raylib::IsWindowReady())
            return nullptr;
        RayWindow *created = new RayWindow(screenWidth, screenHeight, title);

        _window = created;
        return created;
    }
    void deleteWindow(graphic::IWindow2 *window) override {
        if (window == _window)
            _window = nullptr;
        delete window;
    }

    graphic::IWindow2 *window() override { return _window; }

    // input - the window is ignored : raylib is C, its input state is
    // global and it only accepts one window anyway
    graphic::IKeyboard *createKeyboard(graphic::IWindow *) override { return new RayKeyboard(); }
    void deleteKeyboard(graphic::IKeyboard *keyboard) override { delete keyboard; }

    graphic::IMouse *createMouse(graphic::IWindow *) override { return new RayMouse(); }
    void deleteMouse(graphic::IMouse *mouse) override { delete mouse; }

    graphic::IGamepad *createGamepad(graphic::IWindow *) override { return new RayGamepad(0); }
    void deleteGamepad(graphic::IGamepad *gamepad) override { delete gamepad; }

    // texture
    graphic::ITexture *createTexture(std::string path) override {
        return new RayTexture(path);
    }
    void deleteTexture(graphic::ITexture *texture) override {
        delete texture;
    }

    // font
    graphic::IFont *createFont(std::string path) override {
        return new RayFont(path);
    }
    void deleteFont(graphic::IFont *font) override {
        delete font;
    }

    // polygon
    graphic::IPolygon *createPolygon(std::vector<Vector2f> points) override {
        return new RayPolygon(points);
    }
    void deletePolygon(graphic::IPolygon *polygon) override {
        delete polygon;
    }

    // sprite
    graphic::ISprite *createSprite(graphic::ITexture *texture) override {
        return new RaySprite(*static_cast<RayTexture *>(texture));
    }
    void deleteSprite(graphic::ISprite *sprite) override {
        delete sprite;
    }

    // text
    graphic::IText *createText(std::string text, graphic::IFont *font) override {
        return new RayText(text, *static_cast<RayFont *>(font));
    }
    void deleteText(graphic::IText *text) override {
        delete text;
    }

    // camera
    graphic::ICamera *createCamera(Vector3f position, Vector3f target, float fov) override {
        return new RayCamera(position, target, fov);
    }
    void deleteCamera(graphic::ICamera *camera) override {
        delete camera;
    }

    // mesh
    graphic::IMesh *createMesh(std::string path) override {
        return new RayMesh(path);
    }
    void deleteMesh(graphic::IMesh *mesh) override {
        delete mesh;
    }

    // animation set
    graphic::IAnimationSet *createAnimationSet(std::string path) override {
        return new RayAnimationSet(path);
    }
    void deleteAnimationSet(graphic::IAnimationSet *animations) override {
        delete animations;
    }

    // model
    graphic::IModel *createModel(graphic::IMesh *mesh) override {
        return new RayModel(*static_cast<RayMesh *>(mesh));
    }
    void deleteModel(graphic::IModel *model) override {
        delete model;
    }

private:
    /// La seule fenetre que raylib sache ouvrir : celle que window() prete.
    RayWindow *_window = nullptr;
};

/** @} */

#endif /* !RAYGRAPHIC_MODULE_HPP */
