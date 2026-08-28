#include <gtest/gtest.h>

#include "ModuleManager.hpp"
#include "IGraphic2Module.hpp"
#include "IGraphic3Module.hpp"
#include "IAudioModule.hpp"

#include <string>

struct RaylibImplTest : ::testing::Test {
    ModuleManager<IGraphic2Module, IGraphic3Module, IAudioModule> modules;
};

TEST_F(RaylibImplTest, LoadsAndExposesAllThreeContracts) {
    ASSERT_TRUE(modules.Load(RAYLIB_IMPL_PATH, "raylib"));

    IGraphic2Module *graphic2 = modules.Get<IGraphic2Module>("raylib");
    IGraphic3Module *graphic3 = modules.Get<IGraphic3Module>("raylib");
    IAudioModule *audioModule = modules.Get<IAudioModule>("raylib");

    ASSERT_NE(graphic2, nullptr);
    ASSERT_NE(graphic3, nullptr);
    ASSERT_NE(audioModule, nullptr);
    EXPECT_STREQ(graphic2->name(), "raylib");

    // IGraphic3Module extends IGraphic2Module : same underlying instance
    EXPECT_EQ(static_cast<IGraphic2Module *>(graphic3), graphic2);
}

TEST_F(RaylibImplTest, FullWalkthrough) {
    ASSERT_TRUE(modules.Load(RAYLIB_IMPL_PATH, "raylib"));
    IGraphic3Module *graphic = modules.Get<IGraphic3Module>("raylib");
    IAudioModule *audio = modules.Get<IAudioModule>("raylib");
    ASSERT_NE(graphic, nullptr);
    ASSERT_NE(audio, nullptr);

    graphic::IWindow3 *window = graphic->createWindow(200, 200, "raylib_impl test");
    ASSERT_NE(window, nullptr);
    window->setFrameLimit(60);

    // texture/font are shared, independent of the sprites/text built from them
    graphic::ITexture *texture = graphic->createTexture(std::string(ASSETS_DIR) + "/image.png");
    graphic::IFont *font = graphic->createFont(std::string(ASSETS_DIR) + "/font.ttf");
    ASSERT_TRUE(texture->isReady());
    ASSERT_TRUE(font->isReady());

    graphic::ISprite *sprite = graphic->createSprite(texture);
    graphic::IText *text = graphic->createText("hello raylib_impl", font);
    graphic::IPolygon *polygon = graphic->createPolygon({{0, 100}, {100, 0}, {200, 100}, {100, 200}});
    ASSERT_NE(sprite, nullptr);
    ASSERT_NE(text, nullptr);
    ASSERT_NE(polygon, nullptr);

    // deleting a sprite never touches the texture it came from
    graphic->deleteSprite(sprite);
    sprite = graphic->createSprite(texture);
    ASSERT_TRUE(texture->isReady());

    // mesh/animation set are shared, independent of the model built from them
    graphic::IMesh *mesh = graphic->createMesh(std::string(ASSETS_DIR) + "/fox.glb");
    graphic::IAnimationSet *animations = graphic->createAnimationSet(std::string(ASSETS_DIR) + "/fox.glb");
    ASSERT_TRUE(mesh->isReady());
    ASSERT_TRUE(animations->isReady());
    EXPECT_GT(animations->getSize(), 0);

    graphic::IModel *model = graphic->createModel(mesh);
    ASSERT_NE(model, nullptr);
    model->setTexture(texture);
    model->setAnimations(animations);
    model->setAnimation(0);

    graphic::ICamera *camera = graphic->createCamera({50.f, 50.f, 50.f}, {0.f, 0.f, 0.f}, 45);
    ASSERT_NE(camera, nullptr);

    graphic::IKeyboard *keyboard = graphic->createKeyboard(window);
    graphic::IMouse *mouse = graphic->createMouse(window);
    graphic::IGamepad *gamepad = graphic->createGamepad(window);
    ASSERT_NE(keyboard, nullptr);
    ASSERT_NE(mouse, nullptr);
    ASSERT_NE(gamepad, nullptr);

    audio::ISoundBuffer *soundBuffer = audio->createSoundBuffer(std::string(ASSETS_DIR) + "/step.wav");
    audio::IMusic *music = audio->createMusic(std::string(ASSETS_DIR) + "/ambience.wav");
    ASSERT_TRUE(soundBuffer->isReady());
    ASSERT_TRUE(music->isReady());

    audio::ISound *sound = audio->createSound(soundBuffer);
    ASSERT_NE(sound, nullptr);
    sound->setPosition({1, 2, 3}); // graceful no-op, raylib has no positional audio

    // a handful of frames, bounded - not a real-time loop
    for (int frame = 0; frame < 5; frame++) {
        window->pollEvent();
        window->eventClose();
        keyboard->isKeyDown(graphic::IKeyboard::KEY_SPACE);
        mouse->getPosition();
        model->updateAnimation();

        window->beginDraw();
        window->drawPoly(polygon);
        window->drawSprite(sprite);
        window->drawText(text);

        window->beginMode3(camera);
        window->drawModel(model);
        window->drawText3D(text, {0, 1, 0});
        window->endMode3();

        window->endDraw();
        music->update();
    }

    sound->play();
    music->play();

    audio->deleteSound(sound);
    audio->deleteSoundBuffer(soundBuffer);
    audio->deleteMusic(music);

    graphic->deleteCamera(camera);
    graphic->deleteModel(model);
    graphic->deleteMesh(mesh);
    graphic->deleteAnimationSet(animations);
    graphic->deletePolygon(polygon);
    graphic->deleteSprite(sprite);
    graphic->deleteText(text);
    graphic->deleteTexture(texture);
    graphic->deleteFont(font);

    graphic->deleteKeyboard(keyboard);
    graphic->deleteMouse(mouse);
    graphic->deleteGamepad(gamepad);
    graphic->deleteWindow(window);
}

