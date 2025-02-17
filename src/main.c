#include "main.h"
#include "raylib.h"
#include "utils.h"
#include <stdio.h>

#define CELL_COUNT 4
#define CELL_BLINK_TIME 0.25f
#define NOTE_COUNT 16
#define NOTE_SPEED 600.0f

typedef struct {
  int spawn_tick;
  int column;
  bool spawned;
  int position;
} note;

const float BPM = 60.0f;
const float interval = 60.0f / BPM;
float timer = 0;
bool visible = true;

float global_tick = 0.0f;

Sound metronome;
Sound hitsound;

float cells_timer[CELL_COUNT] = {0};
note notes[NOTE_COUNT] = {0};

const int board_width = 800;
const int cell_padding = 10;
const int cell_width = (board_width - 10) / CELL_COUNT - cell_padding;
const int cell_height = 150;
const int cell_top = 800 - cell_height;

void init(void) {
  InitAudioDevice();
  Font loaded_font = GetFontDefault();
  SetDefaultFont(loaded_font);
  metronome = LoadSound("sounds/Perc_Snap_hi.wav");
  hitsound = LoadSound("sounds/Synth_Bell_A_hi.wav");

  notes[0] = (note){0, 0, 0, 0};
  notes[1] = (note){1, 1, 0, 0};
  notes[2] = (note){1, 2, 0, 0};
  notes[3] = (note){2, 0, 0, 0};
  notes[4] = (note){3, 2, 0, 0};
  notes[5] = (note){3, 1, 0, 0};
  notes[6] = (note){4, 0, 0, 0};
  notes[7] = (note){5, 0, 0, 0};
  notes[8] = (note){6, 1, 0, 0};
  notes[9] = (note){7, 2, 0, 0};
  notes[10] = (note){8, 3, 0, 0};
  notes[11] = (note){9, 3, 0, 0};
  notes[12] = (note){10, 0, 0, 0};
  notes[13] = (note){10, 3, 0, 0};
  notes[14] = (note){11, 1, 0, 0};
  notes[15] = (note){12, 0, 0, 0};
}

void draw_board(void) {

  Rectangle outline = {1200.0f / 2 - 400, 50, board_width, 800 - 50};
  DrawRectangleLinesEx(outline, 5, WHITE);

  for (int i = 0; i < CELL_COUNT; i++) {
    Rectangle pos = {outline.x + (cell_width + cell_padding) * i + 10,
                     outline.y + outline.height - cell_height - 10, cell_width,
                     cell_height};
    DrawRectangleLinesEx(pos, 5, GRAY);
    if (cells_timer[i] > 0) {
      Rectangle inner = {outline.x + (cell_width + cell_padding) * i + 10 + 5,
                         outline.y + outline.height - cell_height - 5,
                         cell_width - 10, cell_height - 10};
      Color faded_color = ColorAlpha(RED, cells_timer[i] / CELL_BLINK_TIME);
      DrawRectangleRec(inner, faded_color);
    }
  }

  for (int i = 0; i < NOTE_COUNT; i++) {
    if (notes[i].spawned) {
      Rectangle note_pos = {
          outline.x + (cell_width + cell_padding) * notes[i].column + 10,
          outline.y + notes[i].position, cell_width, cell_height};
      DrawRectangleRec(note_pos, BLUE);
    }
  }
}

note *hit_note_in_column(int column) {
  for (int i = 0; i < NOTE_COUNT; i++) {
    if (notes[i].spawned && notes[i].column == column) {
      return &notes[i];
    }
  }
  return NULL;
}

const char *message = NULL;
Color message_color = {0, 0, 0, 0};

void key_hit(int column) {
  cells_timer[column] = CELL_BLINK_TIME;
  PlaySound(hitsound);
  note *hit = hit_note_in_column(column);
  if (hit != NULL) {
    int distance = ABS(cell_top - hit->position);
    printf("distance=%d\n", distance);
    if (distance < 300) {
      hit->spawned = false;
      if (distance <= 75) {
        message = "Parfait";
        message_color = BLUE;
      } else if (distance < 100) {
        message = "Bien";
        message_color = GREEN;
      } else if (distance < 200) {
        message = "Ok";
        message_color = YELLOW;
      } else {
        message = "Nul à chier";
        message_color = RED;
      }
    }
  }
}

void main_loop(void) {
  // Updating
  float dt = GetFrameTime();
  timer += dt;

  if (timer >= interval) {
    visible = !visible;
    PlaySound(metronome);
    timer = 0.0f;
    global_tick++;
  }

  for (int i = 0; i < NOTE_COUNT; i++) {
    if (notes[i].spawn_tick == global_tick) {
      notes[i].spawned = true;
    }
    if (notes[i].spawned) {
      notes[i].position += dt * NOTE_SPEED;
    }
    if (notes[i].position > 800) {
      notes[i].spawned = false;
    }
  }

  for (int i = 0; i < CELL_COUNT; i++) {
    if (cells_timer[i] <= 0) {
      cells_timer[i] = 0;
    } else {
      cells_timer[i] -= dt;
    }
  }

  if (IsKeyPressed(KEY_D)) {
    key_hit(0);
  }
  if (IsKeyPressed(KEY_F)) {
    key_hit(1);
  }
  if (IsKeyPressed(KEY_J)) {
    key_hit(2);
  }
  if (IsKeyPressed(KEY_K)) {
    key_hit(3);
  }

  // Rendering
  BeginDrawing();

  ClearBackground(BLACK);

  if (visible) {
    DrawCircle(100, 100, 10, RED);
  }
  draw_board();

  if (message) {
    DrawText(message, 26, 0, 100, message_color);
  }

  DrawFPS(0, 0);
  EndDrawing();
}
