# raylib_impl

version: 0.1.0

[![Docs](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://p-e-r-r-y.github.io/raylib_impl)

> raylib 5.5 behind [igraphic](https://github.com/P-E-R-R-Y/igraphic) and
> [iaudio](https://github.com/P-E-R-R-Y/iaudio).

Exports `getGraphic2Module()`, `getGraphic3Module()` and `getAudioModule()`.
The first two hand back the same instance seen from both ends of the
inheritance chain — one implementation, no risk of the two views drifting.

## Two ways to take it

| target | artefact | use |
|---|---|---|
| `raylib_impl` | `raylib_impl.dylib` | the plugin, what `ModuleManager::Load()` opens |
| `raylib_impl_static` | `libraylib_impl.a` | linked in, brings its include dirs and the platform frameworks |

Neither follows `BUILD_SHARED_LIBS` : a plugin turned into a `.a` would fail
at runtime with an obscure message. Pick by target name.

## Examples

```bash
build/raylib_impl_demo_dynamic    # dlopen at runtime
build/raylib_impl_demo_static     # linked in, no dlopen at all
```

`dynamic` takes a dll path in argv[1], so the same binary runs any vendor :

```bash
build/raylib_impl_demo_dynamic ../../sfml_impl/build/sfml_impl.dylib ../../sfml_impl/assets
```

## What raylib forced

- **No event queue at all**, only state queries refreshed once a frame by
  `PollInputEvents()` inside `EndDrawing()`. `IsKeyPressed` compares two
  arrays, so nothing is consumed and every read is idempotent — this is the
  behaviour the contract asks for, and the one sfml was aligned on. Here
  `pollEvent()` has nothing to drain and answers `true` always, since raylib
  cannot tell whether anything happened.
- `IsMusicStreamPlaying` cannot tell "paused" from "stopped", so `RayMusic`
  tracks the three states itself.
- `SeekMusicStream` wraps past the end (115s on a 109s track gave 6s), so
  `setTime` clamps.
- `PlaySound` overlaps by default ; the contract wants a restart.
- No modifier mask on events : rebuilt from `IsKeyDown`.
- No positional audio : `setPosition` stores and keeps playing.

## Known limitation

`UpdateModelAnimation` writes the pose into the mesh's own vertex buffer, so
two `RayModel` sharing one `RayMesh` cannot sit on different animation
frames. Sharing position, rotation and size works.

## Tests need a real screen

`InitWindow` segfaults inside raylib when the display is asleep or locked —
GLFW finds no monitor and raylib dereferences null GL pointers. On CI, use
`xvfb-run`.
