#if defined(PLATFORM_WEB)
#include "main.h"
#include "raylib.h"
#include <emscripten/emscripten.h>
#include <stdio.h>

int main(void) {
  InitWindow(1200, 800, "Rythm Duel");
  init();
  emscripten_set_main_loop(main_loop, 0, 1);
}
#else
void empty_pedantic(void) {}
#endif
