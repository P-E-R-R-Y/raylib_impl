/**
 * @file RayWindow.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-10
 *
 * @addtogroup raylib
 * @{
 */

#ifndef RAYWINDOW_HPP_
#define RAYWINDOW_HPP_

//Raylib (without conflict)
#include "RaylibAliases.hpp"

//Interface
#include "IWindow3.hpp"

//encapsulation
#include "RayCamera.hpp"
#include "RayModel.hpp"
#include "RayPolygon.hpp"
#include "RaySprite.hpp"
#include "RayText.hpp"

/**
 * @brief Raylib Window class - a single concrete class implementing
 *        IWindow3 fully (which already IS-A IWindow2 IS-A IWindow via the
 *        interface chain), matching raylib itself : one native window, no
 *        separate 2D/3D window objects.
 *
 * raylib switches to 3D through a global mode (BeginMode3D), not through
 * a second object : splitting into three C++ types would describe nothing
 * real. The active camera is held between beginMode3() and endMode3().
 * */
class RayWindow : public graphic::IWindow3 {

    public:
        RayWindow(int32_t screenWidth, int32_t screenHeight, std::string title) : _quitRequested(false) {
            raylib::InitWindow(screenWidth, screenHeight, title.c_str());
        }

        ~RayWindow() {
            /* Le curseur de raylib est GLOBAL au processus, pas attache a
             * la fenetre : partir en le laissant cache priverait de pointeur
             * la borne, et le bureau avec elle. */
            raylib::ShowCursor();
            raylib::CloseWindow();
        }

        //lifecycle
        bool isOpen() override {
            return raylib::IsWindowReady() && !raylib::WindowShouldClose() && !_quitRequested;
        }

        void close() override {
            _quitRequested = true;
        }

        Vector2f getPosition() override {
            const raylib::Vector2 position = raylib::GetWindowPosition();
            return {static_cast<double>(position.x), static_cast<double>(position.y)};
        }

        void setPosition(Vector2f position) override {
            raylib::SetWindowPosition(static_cast<int>(position.x), static_cast<int>(position.y));
        }

        Vector2f getSize() override {
            return {static_cast<double>(raylib::GetScreenWidth()),
                    static_cast<double>(raylib::GetScreenHeight())};
        }

        void setSize(Vector2f size) override {
            raylib::SetWindowSize(static_cast<int>(size.x), static_cast<int>(size.y));
        }

        void setFrameLimit(int32_t limit) override {
            raylib::SetTargetFPS(limit);
        }

        void setMouseVisibility(bool visible) override {
            if (visible)
                raylib::ShowCursor();
            else
                raylib::HideCursor();
        }

        int32_t getDelta() override {
            return static_cast<int32_t>(raylib::GetFrameTime() * 1000);
        }

        /**
         * @brief raylib has NO event queue : only state queries, refreshed
         *        once per frame by PollInputEvents() inside EndDrawing().
         *
         * There is nothing to drain here, and nothing to guard against a
         * second caller : the state is already frozen for the frame and
         * every read is idempotent.
         *
         * It answers true always, because raylib has no way to tell whether
         * anything happened. Harmless as long as only the fronts sit inside
         * the condition - with no event they are false anyway. See IWindow.
         */
        bool pollEvent() override {
            return true;
        }

        void eventClose() override {
            //no-op : the close condition is already checked in isOpen()
        }

        //2D
        void beginDraw() override {
            raylib::BeginDrawing();
            raylib::ClearBackground({0, 0, 0, 255});
        }

        void endDraw() override {
            raylib::EndDrawing();
        }

        void drawPoly(graphic::IPolygon *polygon) override;
        void drawSprite(graphic::ISprite *sprite) override;
        void drawText(graphic::IText *text) override;

        //3D
        void beginMode3(graphic::ICamera *camera) override;
        void drawModel(graphic::IModel *model) override;
        void drawText3D(graphic::IText *text, Vector3f position) override;
        void endMode3() override;

    private:
        bool _quitRequested;
        raylib::Camera *_activeCamera = nullptr;
};

//2D

void RayWindow::drawPoly(graphic::IPolygon *polygon) {
    RayPolygon *rayPolygon = static_cast<RayPolygon *>(polygon);
    // by reference : copying the vector every frame would be a per-frame
    // allocation for nothing, the triangles are computed at construction
    const std::vector<Triangle<double>> &triangles = rayPolygon->_triangles;

    for (size_t i = 0; i < triangles.size(); i++) {
        //raylib asks for counter clockwise, hence the reversed order
        raylib::DrawTriangle(
            {static_cast<float>(triangles[i].p3.x + rayPolygon->getPosition().x), static_cast<float>(triangles[i].p3.y + rayPolygon->getPosition().y)},
            {static_cast<float>(triangles[i].p2.x + rayPolygon->getPosition().x), static_cast<float>(triangles[i].p2.y + rayPolygon->getPosition().y)},
            {static_cast<float>(triangles[i].p1.x + rayPolygon->getPosition().x), static_cast<float>(triangles[i].p1.y + rayPolygon->getPosition().y)},
            rayPolygon->_color);
    }
}

void RayWindow::drawSprite(graphic::ISprite *sprite) {
    RaySprite *raySprite = static_cast<RaySprite *>(sprite);
    raylib::Rectangle posSize = {float(raySprite->getPosition().x), float(raySprite->getPosition().y),
                                  float(raySprite->getSize().x), float(raySprite->getSize().y)};

    raylib::DrawTexturePro(raySprite->_texture.handle(), raySprite->_crop, posSize, {0, 0}, raySprite->_rotation,
                            {255, 255, 255, 255});
}

void RayWindow::drawText(graphic::IText *text) {
    RayText *rayText = static_cast<RayText *>(text);

    raylib::DrawTextPro(rayText->_font->handle(), rayText->_data.c_str(), rayText->_position, {0, 0},
                         rayText->_rotation, rayText->_size, rayText->_spacing, rayText->_color);
}

//3D

void RayWindow::beginMode3(graphic::ICamera *camera) {
    RayCamera *rayCamera = static_cast<RayCamera *>(camera);

    raylib::UpdateCamera(&rayCamera->_camera, rayCamera->_mode);
    _activeCamera = &rayCamera->_camera;

    raylib::BeginMode3D(rayCamera->_camera);
}

void RayWindow::drawModel(graphic::IModel *model) {
    RayModel *rayModel = static_cast<RayModel *>(model);

    raylib::DrawModelEx(rayModel->_mesh.handle(), rayModel->_position, {0, 1, 0}, 0, rayModel->_scale, raylib::WHITE);
}

void RayWindow::drawText3D(graphic::IText *text, Vector3f position) {
    //raylib has no built-in DrawText3D : project the world position through
    //the active camera and draw the same 2D text there (screen-space billboard).
    //2D draw calls need orthographic mode, so step out of BeginMode3D and
    //back in around the projected draw - the caller's own endMode3() closes
    //the block they think they're still in.
    RayText *rayText = static_cast<RayText *>(text);
    if (!_activeCamera) return;

    raylib::Vector3 worldPos = {float(position.x), float(position.y), float(position.z)};
    raylib::Vector2 screenPos = raylib::GetWorldToScreen(worldPos, *_activeCamera);

    raylib::EndMode3D();
    raylib::DrawTextPro(rayText->_font->handle(), rayText->_data.c_str(), screenPos, {0, 0}, rayText->_rotation,
                         rayText->_size, rayText->_spacing, rayText->_color);
    raylib::BeginMode3D(*_activeCamera);
}

void RayWindow::endMode3() {
    raylib::EndMode3D();
    _activeCamera = nullptr;
}

/** @} */

#endif /* !RAYWINDOW_HPP_ */
