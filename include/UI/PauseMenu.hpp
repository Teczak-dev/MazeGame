#pragma once

#include "raylib.h"

enum class PauseMenuAction { None, Resume, MainMenu, Restart, Exit };

// Menu pauzy nie zmienia mapy ani czasu - tylko wybiera dalszy kierunek gry.
class PauseMenu {
public:
  PauseMenuAction Update() const;
  void Draw() const;
};
