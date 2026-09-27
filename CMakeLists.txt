cmake_minimum_required(VERSION 3.20)
project(CSafeMEMZ C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(CSafeMEMZ WIN32
    src/main.c
    src/core/engine.c
    src/core/event.c
    src/core/timeline.c
    src/effects/effects.c
    src/ui/fake_windows.c
    src/ui/intro.c
    src/ui/outro.c
)

target_include_directories(CSafeMEMZ PRIVATE include src/core src/effects src/ui)

target_link_libraries(CSafeMEMZ PRIVATE user32 gdi32)

