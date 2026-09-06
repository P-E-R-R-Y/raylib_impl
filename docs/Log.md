# raylib_impl — changelog

Markers: 🟢 added · 🔴 breaking · 🔵 fix · ⚪ internal or docs · 🟡 proposed
in the plan, no code written yet.

## v0.2.0

- 🟢 `RayGraphicModule::claims()` revendique `"opengl"` : raylib tient son
  contexte, sa fenetre et ses entrees dans ses propres globales, rien ne
  peut cohabiter avec lui.
- 🔴 Suit imodule v0.2.0, qui absorbe `modulemanager`.

## v0.1.0

- 🟢 fills `graphic3` + `audio` (so `graphic2` via the `accepts` chain)
- 🟢 single entry point `getModules()`
- 🟢 SHARED and STATIC targets (STATIC recompiles `sources/raylib.cpp` for
  a consumer that doesn't need it — known, not fixed)

### Measured failure, not fixed

- 🔴 `createTexture`/`createSoundBuffer` etc. return a dead object
  (`isReady()==false`) instead of `nullptr` when loading fails.
  `createSound` on a dead object segfaults (SIGSEGV, exit 139)

## Proposed, not written

- 🟡 `claims()`: `{"opengl"}` — sfml and raylib excluded without naming each other
- 🟡 STATIC target → INTERFACE, zero object code
