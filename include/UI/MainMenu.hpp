#pragma once

#include "raylib.h"

enum class MainMenuAction { None, Start, Exit };

// Prosty ekran startowy z przyciskami i ustawieniami okna.
class MainMenu {
public:
  MainMenuAction Update();
  void Draw() const;

  int GetSelectedWidth() const;
  int GetSelectedHeight() const;
  bool IsFullscreenSelected() const;
  int GetMazeWidth() const;
  int GetMazeHeight() const;

private:
  int resolutionIndex = 1;
  int mazeSizeIndex = 1;
  bool fullscreen = false;
};
