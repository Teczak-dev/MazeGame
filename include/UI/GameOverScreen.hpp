#pragma once

#include "raylib.h"

enum class GameOverAction { None, Retry, Exit };

// Ekran pokazywany po dotarciu do wyjscia.
class GameOverScreen {
public:
  GameOverScreen() = default;
  explicit GameOverScreen(double completionTime);

  GameOverAction Update() const;
  void Draw() const;

private:
  double completionTime;
};
