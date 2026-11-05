#include "UI/GameOverScreen.hpp"

namespace {
Rectangle RetryButton() {
  return {GetScreenWidth() / 2.0f - 170.0f, 490.0f, 340.0f, 60.0f};
}

Rectangle ExitButton() {
  return {GetScreenWidth() / 2.0f - 170.0f, 570.0f, 340.0f, 60.0f};
}

bool Clicked(Rectangle rectangle) {
  return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
         CheckCollisionPointRec(GetMousePosition(), rectangle);
}

void DrawButton(Rectangle rectangle, const char *label) {
  DrawRectangleRec(rectangle, {45, 50, 75, 255});
  DrawRectangleLinesEx(rectangle, 2.0f, {190, 60, 255, 255});
  DrawText(label, static_cast<int>(rectangle.x + 45),
           static_cast<int>(rectangle.y + 17), 24, RAYWHITE);
}
} // namespace

GameOverScreen::GameOverScreen(double completionTime)
    : completionTime(completionTime) {}

GameOverAction GameOverScreen::Update() const {
  if (IsKeyPressed(KEY_ENTER) || Clicked(RetryButton())) {
    return GameOverAction::Retry;
  }
  if (IsKeyPressed(KEY_ESCAPE) || Clicked(ExitButton())) {
    return GameOverAction::Exit;
  }
  return GameOverAction::None;
}

void GameOverScreen::Draw() const {
  const int screenWidth = GetScreenWidth();
  const int milliseconds = static_cast<int>(completionTime * 1000.0) % 1000;
  const int totalSeconds = static_cast<int>(completionTime);
  const int minutes = totalSeconds / 60;
  const int seconds = totalSeconds % 60;

  const char *title = "Brawo!";
  DrawRectangle(80, 100, screenWidth - 160, 560, {12, 14, 25, 235});
  DrawRectangleLinesEx({80, 100, static_cast<float>(screenWidth - 160), 560},
                       3.0f, {190, 60, 255, 255});
  DrawText(title, (screenWidth - MeasureText(title, 64)) / 2, 140, 64,
           {220, 190, 255, 255});
  const char *message =
      "udalo ci sie odnalezc wyjscie z podziemnego wiezienia";
  DrawText(message, (screenWidth - MeasureText(message, 24)) / 2, 250, 24,
           RAYWHITE);
  const char *timeText =
      TextFormat("Czas: %02d:%02d:%03d", minutes, seconds, milliseconds);
  DrawText(timeText, (screenWidth - MeasureText(timeText, 32)) / 2, 340, 32,
           GOLD);

  DrawButton(RetryButton(), "SPROBUJ PONOWNIE");
  DrawButton(ExitButton(), "WYJDZ");
}
