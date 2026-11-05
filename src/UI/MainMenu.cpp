#include "UI/MainMenu.hpp"

namespace {
const int resolutions[][2] = {{1280, 720}, {1600, 900}, {1920, 1080}};
const int resolutionCount = sizeof(resolutions) / sizeof(resolutions[0]);
const int mazeSizes[][2] = {{21, 21}, {41, 41}, {61, 61}, {101, 101}};
const char *mazeTimes[] = {"~1 minuta", "~5 minut", "10-20 minut", "20+ minut"};
const int mazeSizeCount = sizeof(mazeSizes) / sizeof(mazeSizes[0]);

Rectangle StartButton() {
  return {GetScreenWidth() / 2.0f - 150.0f, 330.0f, 300.0f, 60.0f};
}

Rectangle ExitButton() {
  return {GetScreenWidth() / 2.0f - 150.0f, 490.0f, 300.0f, 60.0f};
}

Rectangle MazeSizeButton() {
  return {GetScreenWidth() / 2.0f - 250.0f, 410.0f, 500.0f, 50.0f};
}

bool Clicked(Rectangle rectangle) {
  return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
         CheckCollisionPointRec(GetMousePosition(), rectangle);
}

void DrawButton(Rectangle rectangle, const char *label) {
  const bool hovered =
      CheckCollisionPointRec(GetMousePosition(), rectangle);
  DrawRectangleRec(rectangle, hovered ? Color{70, 90, 130, 255}
                                     : Color{45, 50, 75, 255});
  DrawRectangleLinesEx(rectangle, 2.0f, {190, 60, 255, 255});
  const int textWidth = MeasureText(label, 24);
  DrawText(label, static_cast<int>(rectangle.x + (rectangle.width - textWidth) / 2),
           static_cast<int>(rectangle.y + 17), 24, RAYWHITE);
}
} // namespace

MainMenuAction MainMenu::Update() {
  const Rectangle startButton = StartButton();
  const Rectangle exitButton = ExitButton();

  if (IsKeyPressed(KEY_ENTER) || Clicked(startButton)) {
    return MainMenuAction::Start;
  }
  if (IsKeyPressed(KEY_ESCAPE) || Clicked(exitButton)) {
    return MainMenuAction::Exit;
  }

  if (IsKeyPressed(KEY_LEFT)) {
    resolutionIndex =
        (resolutionIndex + resolutionCount - 1) % resolutionCount;
  }
  if (IsKeyPressed(KEY_RIGHT)) {
    resolutionIndex = (resolutionIndex + 1) % resolutionCount;
  }
  if (IsKeyPressed(KEY_A)) {
    mazeSizeIndex = (mazeSizeIndex + mazeSizeCount - 1) % mazeSizeCount;
  }
  if (IsKeyPressed(KEY_D)) {
    mazeSizeIndex = (mazeSizeIndex + 1) % mazeSizeCount;
  }
  if (IsKeyPressed(KEY_F)) {
    fullscreen = !fullscreen;
  }

  return MainMenuAction::None;
}

void MainMenu::Draw() const {
  ClearBackground({12, 14, 25, 255});
  const int screenWidth = GetScreenWidth();
  const int screenHeight = GetScreenHeight();

  const char *title = "MAZE GAME";
  DrawText(title, (screenWidth - MeasureText(title, 64)) / 2, 130, 64,
           {220, 190, 255, 255});
  DrawText("Podziemne wiezienie", (screenWidth - MeasureText("Podziemne wiezienie", 24)) / 2,
           210, 24, LIGHTGRAY);

  DrawButton(StartButton(), "START");
  const char *mazeText = TextFormat(
      "Rozmiar labiryntu: %dx%d  |  czas: %s (A/D)",
      mazeSizes[mazeSizeIndex][0], mazeSizes[mazeSizeIndex][1],
      mazeTimes[mazeSizeIndex]);
  DrawRectangleRec(MazeSizeButton(), {30, 35, 55, 255});
  DrawRectangleLinesEx(MazeSizeButton(), 2.0f, {190, 60, 255, 255});
  DrawText(mazeText, static_cast<int>(MazeSizeButton().x + 15),
           static_cast<int>(MazeSizeButton().y + 14), 20, RAYWHITE);
  DrawButton(ExitButton(), "WYJDZ");

  const char *resolutionText =
      TextFormat("Rozdzielczosc: %dx%d (LEWO/PRAWO)",
                 resolutions[resolutionIndex][0],
                 resolutions[resolutionIndex][1]);
  DrawText(resolutionText, 30, screenHeight - 90, 20, RAYWHITE);
  DrawText(TextFormat("Pelny ekran: %s (F)",
                      fullscreen ? "TAK" : "NIE"),
           30, screenHeight - 55, 20, RAYWHITE);
}

int MainMenu::GetSelectedWidth() const {
  return resolutions[resolutionIndex][0];
}

int MainMenu::GetSelectedHeight() const {
  return resolutions[resolutionIndex][1];
}

bool MainMenu::IsFullscreenSelected() const { return fullscreen; }

int MainMenu::GetMazeWidth() const { return mazeSizes[mazeSizeIndex][0]; }

int MainMenu::GetMazeHeight() const { return mazeSizes[mazeSizeIndex][1]; }
