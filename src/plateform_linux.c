#if defined(PLATFORM_DESKTOP)
#include "main.h"
#include "raylib.h"

int main(void) {
  InitWindow(1200, 800, "Rythm Duel");
  SetTargetFPS(60);

  init();
  while (!WindowShouldClose()) {
    main_loop();
  }

  // TODO: Proper exiting
}
#else
void empty_pedantic(void) {}
#endif
