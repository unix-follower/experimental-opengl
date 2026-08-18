# Copilot instructions for experimental-opengl

A small C++23 OpenGL sandbox: a single executable that runs one of several
independent "demo" scenes selected via a CLI flag, backed by a shared static
library of GL helper classes.

## Build, configure, test

Dependencies are managed by vcpkg (manifest mode, `vcpkg.json`) and require
`VCPKG_ROOT` to be set. The full clean-build-test cycle used in CI is:

```bash
./build.sh
# equivalent to:
vcpkg install
cmake --preset=vcpkg
cmake --build --preset=vcpkg
ctest --preset=vcpkg
```

- Build output goes to `build/` (Ninja generator, configured via
  `CMakePresets.json` preset `vcpkg`, which points `CMAKE_TOOLCHAIN_FILE` at
  vcpkg).
- The executable is built as `experimental_opengl_app` (target name
  `experimental_opengl`, renamed via `OUTPUT_NAME`).
- Shaders are copied from `shaders/` into `build/shaders/` by the
  `copy_shaders()` CMake helper in `cmake/Utils.cmake` (added as an `ALL`
  custom target named `shaders`) — edit shader sources in the top-level
  `shaders/` directory, not under `build/`.
- Run a single demo: `./build/experimental_opengl_app -n <number>` (1–5; see
  `demoRegistry()` in `src/main.cpp` for the mapping to
  `basic_shapes::show*` functions declared in `src/basic_shapes.hpp`).
- Testing: `test/` contains a GoogleTest target (`basic_tests`) but it is
  **not currently wired into the top-level `CMakeLists.txt`** (no
  `add_subdirectory(test)`), and `test/main_test.cpp` is empty. `ctest` will
  find no tests until this is connected; if you add tests, add
  `add_subdirectory(test)` to the root `CMakeLists.txt` first, and note
  `test/CMakeLists.txt` currently links against a target named `app_core`,
  which must match whatever the core library target is named
  (`experimental_opengl_core` in the current `CMakeLists.txt`).

## Formatting and static analysis (enforced in CI, `.github/workflows/build.yaml`)

```bash
# Format (matches CI's dry-run check)
git ls-files '*.cpp' '*.hpp' '*.cc' '*.cxx' '*.vert' '*.frag' | xargs clang-format -i

# Static analysis (run after configuring, needs build/compile_commands.json)
git ls-files '*.cpp' '*.hpp' '*.cc' '*.cxx' | xargs clang-tidy -p build
```

Style is defined by `.clang-format` (LLVM-based, 4-space indent, 100 col
limit, left-aligned pointers) and `.clang-tidy` (clang-diagnostic, clang-analyzer,
bugprone, performance, readability checks, with several readability checks
disabled — see `.clang-tidy` for the exact list).

## Architecture

- `src/main.cpp` is the single entry point. It parses `-n/--num` via
  `cxxopts` and dispatches to one demo function from `demoRegistry()`. Adding
  a new demo means: implement it as a new `.cpp`/function pair declared in
  `basic_shapes.hpp`, register it in `CMakeLists.txt`'s
  `experimental_opengl_core` sources, and add an entry to `demoRegistry()`.
- All demos and errors funnel through `main.cpp`'s `try`/`catch`, which uses
  `boost::stacktrace` (`printTrace`) to print Java-style stack traces on
  uncaught exceptions — this is why `BOOST_STACKTRACE_USE_BACKTRACE` and
  `Boost::stacktrace_backtrace` are linked into the core library.
- GL boilerplate is factored into small RAII-ish wrapper classes, each with a
  matching `.hpp`/`.cpp` pair: `Shader` (`shader.hpp/.cpp`, compiles/links
  and reports GLSL compile/link errors), `VertexArrayObject`,
  `VertexBufferObject`, `ElementBufferObject`, and `Texture`
  (`stb_image`-backed, implemented via `stb_image_impl.cpp`). Each demo
  (`equilateral_triangle.cpp`,
  `equilateral_triangle_with_inscribed_upside_down_eq_triangle.cpp`,
  `project_layout_example.cpp`, `shaders_demo.cpp`,
  `example_with_textures.cpp`) composes these wrappers rather than calling
  raw GL functions directly.
- Shaders live in `shaders/*.vert`/`*.frag` as plain text files loaded at
  runtime (`getFileContent` in `shader.cpp`), not compiled in — they are
  copied next to the executable at build time (see `copy_shaders` above), so
  changes take effect after a rebuild/re-copy, not just a re-run.
- `#include <glad/glad.h>` must always precede `#include <GLFW/glfw3.h>`;
  existing headers wrap this with `// clang-format off/on` to preserve the
  order against `SortIncludes: true`.
