/**
 * @file static.cpp
 * @brief raylib lie EN DUR : utilitaires + vendor + contrats, rien d'autre.
 *
 * Pas de dll, pas de symbole d'entree, pas de ModuleManager - tout ca
 * n'existe que pour retrouver du code dans un binaire separe au runtime,
 * ce qui n'a aucun sens quand le code est deja dans l'executable.
 *
 * Les classes concretes sont nommees ici, une seule fois, pour construire.
 * Tout de suite apres, on ne manipule plus que des contrats : runScene ne
 * connait ni raylib ni RayWindow, et c'est le meme fichier que dans
 * sfml_impl - et le meme que celui qu'appelle dynamic.cpp.
 *
 * unique_ptr plutot que new/delete : le module fournissait des paires
 * createX/deleteX qu'il fallait appeler dans le bon ordre. Ici la
 * destruction est automatique, en ordre inverse de declaration, y compris
 * si une exception traverse main(). Rien a oublier.
 */

#include <cstdio>
#include <memory>
#include <string>

#include "RayAnimationSet.hpp"
#include "RayCamera.hpp"
#include "RayFont.hpp"
#include "RayGamepad.hpp"
#include "RayKeyboard.hpp"
#include "RayMesh.hpp"
#include "RayModel.hpp"
#include "RayMouse.hpp"
#include "RayMusic.hpp"
#include "RayPolygon.hpp"
#include "RaySound.hpp"
#include "RaySoundBuffer.hpp"
#include "RaySprite.hpp"
#include "RayText.hpp"
#include "RayTexture.hpp"
#include "RayWindow.hpp"

#include "DemoScene.hpp"

int main(int argc, char **argv) {
    const std::string assets = (argc > 1) ? argv[1] : ASSETS_DIR;

    /* Ce que le module faisait sans le dire : raylib exige un
     * InitAudioDevice() avant tout chargement de son, et un
     * CloseAudioDevice() a la fin. RayAudioModule le tenait en RAII. En
     * statique la responsabilite revient a l'appelant - c'est le prix du
     * lien direct, et sfml n'a pas cette etape du tout. */
    struct AudioDevice {
        AudioDevice()  { raylib::InitAudioDevice(); }
        ~AudioDevice() { raylib::CloseAudioDevice(); }
    } device;

    auto window   = std::make_unique<RayWindow>(960, 540, "raylib_impl - static");

    /* raylib n'a pas besoin de la fenetre pour ses entrees : ses requetes
     * sont globales. C'est la seule vraie difference avec sfml_impl. */
    auto keyboard = std::make_unique<RayKeyboard>();
    auto mouse    = std::make_unique<RayMouse>();
    auto gamepad  = std::make_unique<RayGamepad>(0);

    auto texture  = std::make_unique<RayTexture>(assets + "/image.png");
    auto sprite   = std::make_unique<RaySprite>(*texture);
    auto font     = std::make_unique<RayFont>(assets + "/font.ttf");
    auto hud      = std::make_unique<RayText>("", *font);
    auto label    = std::make_unique<RayText>("", *font);
    // un seul rectangle, repositionne pour dessiner chacun des boutons
    auto poly     = std::make_unique<RayPolygon>(
        std::vector<Vector2f>{{-125, -20}, {125, -20}, {125, 20}, {-125, 20}});
    // concave : un sommet sur deux rentre, DrawTriangleFan ne saurait pas
    auto star     = std::make_unique<RayPolygon>(demo::starPoints(5, 90.0, 36.0));

    auto buffer   = std::make_unique<RaySoundBuffer>(assets + "/step.wav");
    /* Deux sons sur le MEME buffer : un objet son est une voix. Rejouer le
     * son 1 le redemarre, jouer le 2 pendant que le 1 tourne les superpose. */
    auto sound1   = std::make_unique<RaySound>(*buffer);
    auto sound2   = std::make_unique<RaySound>(*buffer);
    auto music    = std::make_unique<RayMusic>(assets + "/ambience.wav");

    // raylib fournit la 3D : le meme objet fenetre sert de IWindow2 et de
    // IWindow3, comme raylib lui-meme qui bascule par un mode global.
    auto mesh       = std::make_unique<RayMesh>(assets + "/fox.glb");
    auto animations = std::make_unique<RayAnimationSet>(assets + "/fox.glb");
    auto foxTexture = std::make_unique<RayTexture>(assets + "/fox2.png");
    auto model      = std::make_unique<RayModel>(*mesh);
    auto camera     = std::make_unique<RayCamera>(Vector3f{110.f, 80.f, 110.f},
                                                   Vector3f{0.f, 0.f, 0.f}, 45.f);

    std::printf("[static] raylib lie en dur, aucun dlopen\n");

    Scene scene{
        .window     = *window,
        .window3    = window.get(),
        .keyboard   = *keyboard,
        .mouse      = *mouse,
        .gamepad    = gamepad.get(),
        .texture    = *texture,
        .sprite     = *sprite,
        .font       = *font,
        .hud        = *hud,
        .label      = *label,
        .poly       = *poly,
        .star       = *star,
        .buffer     = *buffer,
        .sound1     = *sound1,
        .sound2     = *sound2,
        .music      = *music,
        .mesh       = mesh.get(),
        .animations = animations.get(),
        .model      = model.get(),
        .camera     = camera.get(),
        .modelTexture = foxTexture.get(),
    };

    return runScene(scene);
}
