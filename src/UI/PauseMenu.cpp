#include "UI/PauseMenu.hpp"

namespace {
Rectangle Button(int number) {
  return {GetScreenWidth() / 2.0f - 180.0f,
          300.0f + number * 75.0f, 360.0f, 55.0f};
}

bool Clicked(Rectangle rectangle) {
  return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
         CheckCollisionPointRec(GetMousePosition(), rectangle);
}

void DrawButton(Rectangle rectangle, const char *label) {
  DrawRectangleRec(rectangle, {45, 50, 75, 255});
  DrawRectangleLinesEx(rectangle, 2.0f, {190, 60, 255, 255});
  DrawText(label, static_cast<int>(rectangle.x + 25),
           static_cast<int>(rectangle.y + 15), 22, RAYWHITE);
}
} // namespace

PauseMenuAction PauseMenu::Update() const {
  if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE) ||
      Clicked(Button(0))) {
    return PauseMenuAction::Resume;
  }
  if (Clicked(Button(1))) {
    return PauseMenuAction::MainMenu;
  }
  if (Clicked(Button(2))) {
    return PauseMenuAction::Restart;
  }
  if (Clicked(Button(3))) {
    return PauseMenuAction::Exit;
  }
  return PauseMenuAction::None;
}

void PauseMenu::Draw() const {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {8, 8, 14, 235});
  const char *title = "PAUZA";
  DrawText(title, (GetScreenWidth() - MeasureText(title, 56)) / 2, 150, 56,
           {220, 190, 255, 255});
  DrawButton(Button(0), "WROC DO GRY");
  DrawButton(Button(1), "MENU GLOWNE");
  DrawButton(Button(2), "ZACZNIJ OD NOWA");
  DrawButton(Button(3), "WYJDZ Z GRY");
}
