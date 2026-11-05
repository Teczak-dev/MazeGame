#include "raylib.h"

int main() {
  // Initialization
  const int screenWidth = 800;
  const int screenHeight = 600;

  InitWindow(screenWidth, screenHeight, "Raylib Example");

  SetTargetFPS(60);

  // Main game loop
  while (!WindowShouldClose()) {
    // Update

    // Draw
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("Hello, Raylib!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
  }

  // De-Initialization
  CloseWindow();

  return 0;
}
