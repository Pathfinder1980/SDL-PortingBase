# SDL-PortingBase

A personal learning project, built from scratch by hand: a minimal SDL2
application with a thin platform layer and SDL-free logic underneath.

## Goals

- **Refresh OpenGL.** Back to the fundamentals on a 3.3 core profile:
  hand-rolled function loading (no GLAD/GLEW), explicit vertex layout,
  shaders, uniforms and the matrix math behind them — no engine doing
  the thinking for me.
- **Real hands-on time with SDL2.** Window, GL context, main loop,
  input and (soon) audio — driven through SDL's actual API, not a
  wrapper found in a tutorial.
- **Learn modern CMake.** Targets, presets, FetchContent, custom
  commands — and using linkage itself to enforce design decisions.

The method for all three: **draw hard architectural boundaries.** Only
`src/platform` may include SDL2 (enforced via PRIVATE linkage, so a
violation is a build error), the core stays SDL-free and unit-tested,
and the renderer talks to GL only through injected function pointers.
Boundaries like these are what make a codebase portable — practicing
them is the point.

## Non-goals

No engine ambitions: no scene graph, no asset pipeline, no abstraction
over more than exactly what the demo needs.

## Build

CMake presets `windows-msvc-debug` / `windows-msvc-release` (Ninja,
MSVC from a Native Tools prompt). Tests run via `ctest`.
