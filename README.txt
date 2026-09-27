CSafeMEMZ final engine/effects/timeline/event set

This version:
- draws visual effects directly onto the desktop DC instead of using a fullscreen overlay window;
- removes the overlay/backbuffer engine;
- introduces a timed sequence with fake Notepad/browser/error/terminal windows;
- keeps all effects visual only;
- ESC exits the simulation.

Replace the corresponding files in the project:
src/core/engine.c
src/core/engine.h
src/core/event.c
src/core/event.h
src/core/timeline.c
src/core/timeline.h
src/effects/effects.c
src/effects/effects.h

If your project has an older event enum, replace the matching headers together with the .c files.
