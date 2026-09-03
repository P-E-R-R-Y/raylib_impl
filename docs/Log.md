# raylib_impl — journal

Marqueurs : 🟢 ajout · 🔴 rupture · 🔵 correctif · ⚪ interne ou doc · 🟡 propose
dans le plan, code non ecrit.

## v0.1.0

- 🟢 remplit `graphic3` + `audio` (donc `graphic2` par la chaine `accepts`)
- 🟢 point d'entree unique `getModules()`
- 🟢 cibles SHARED et STATIC (la STATIC recompile `sources/raylib.cpp` pour
  un consommateur qui n'en a pas besoin — constate, pas corrige)

### Faille mesuree, pas corrigee

- 🔴 `createTexture`/`createSoundBuffer` etc. rendent un objet mort
  (`isReady()==false`) au lieu de `nullptr` quand le chargement echoue.
  `createSound` sur un objet mort segfault (SIGSEGV, exit 139)

## Propose, pas ecrit

- 🟡 `claims()` : `{"opengl"}` — sfml et raylib exclus sans se nommer
- 🟡 cible STATIC → INTERFACE, zero object code
