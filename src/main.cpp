#include "Characters/Player.hpp"
#include "Systems/MazeSystem.hpp"
#include "UI/GameOverScreen.hpp"
#include "UI/MainMenu.hpp"
#include "UI/PauseMenu.hpp"
#include "raylib.h"

int main() {
  const int initialWidth = 1600;
  const int initialHeight = 900;

  InitWindow(initialWidth, initialHeight, "Maze Game");

  SetTargetFPS(60);
  MazeSystem mazeSystem(100, 100);
  Player player(mazeSystem.GetStartPosition().x,
                mazeSystem.GetStartPosition().y);
  Camera2D camera = {0};
  MainMenu mainMenu;
  PauseMenu pauseMenu;
  GameOverScreen *gameOverScreen = nullptr;

  enum class GameState { MainMenu, Playing, Paused, GameOver };
  GameState state = GameState::MainMenu;
  bool mapVisible = false;
  double gameTime = 0.0;

  while (!WindowShouldClose()) {
    if (state == GameState::MainMenu) {
      const MainMenuAction action = mainMenu.Update();
      if (action == MainMenuAction::Start) {
        SetWindowSize(mainMenu.GetSelectedWidth(),
                      mainMenu.GetSelectedHeight());
        if (mainMenu.IsFullscreenSelected() != IsWindowFullscreen()) {
          ToggleFullscreen();
        }
        mazeSystem.SetSize(mainMenu.GetMazeWidth(), mainMenu.GetMazeHeight());
        mazeSystem.Generate();
        player = Player(mazeSystem.GetStartPosition().x,
                        mazeSystem.GetStartPosition().y);
        gameTime = 0.0;
        mapVisible = false;
        state = GameState::Playing;
      } else if (action == MainMenuAction::Exit) {
        break;
      }
    } else if (state == GameState::Playing) {
      if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
        mapVisible = false;
        state = GameState::Paused;
        continue;
      }
      if (IsKeyPressed(KEY_M)) {
        mapVisible = !mapVisible;
      }

      if (!mapVisible) {
        const float deltaTime = GetFrameTime();
        gameTime += deltaTime;
        player.Update(deltaTime, mazeSystem.GetMaze(), 32.0f);
        mazeSystem.Update(deltaTime, player);
        if (mazeSystem.HasPlayerReachedExit(player)) {
          gameOverScreen = new GameOverScreen(gameTime);
          mazeSystem.StartPathReveal();
          state = GameState::GameOver;
        }
      }
    } else if (state == GameState::Paused) {
      const PauseMenuAction action = pauseMenu.Update();
      if (action == PauseMenuAction::Resume) {
        state = GameState::Playing;
      } else if (action == PauseMenuAction::MainMenu) {
        state = GameState::MainMenu;
      } else if (action == PauseMenuAction::Restart) {
        mazeSystem.Generate();
        player = Player(mazeSystem.GetStartPosition().x,
                        mazeSystem.GetStartPosition().y);
        gameTime = 0.0;
        mapVisible = false;
        state = GameState::Playing;
      } else if (action == PauseMenuAction::Exit) {
        break;
      }
    } else {
      const GameOverAction action = gameOverScreen->Update();
      mazeSystem.UpdatePathReveal(GetFrameTime());
      if (action == GameOverAction::Retry) {
        delete gameOverScreen;
        gameOverScreen = nullptr;
        mazeSystem.Generate();
        player = Player(mazeSystem.GetStartPosition().x,
                        mazeSystem.GetStartPosition().y);
        gameTime = 0.0;
        mapVisible = false;
        state = GameState::Playing;
      } else if (action == GameOverAction::Exit) {
        break;
      }
    }

    BeginDrawing();
    if (state == GameState::MainMenu) {
      mainMenu.Draw();
    } else if (state == GameState::Playing) {
      camera.target = player.GetPosition();
      camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
      camera.rotation = 0.0f;
      camera.zoom = 1.0f;
      ClearBackground(BLACK);
      BeginMode2D(camera);
      mazeSystem.Draw();
      player.Draw();
      EndMode2D();

      if (mapVisible) {
        mazeSystem.DrawMapOverlay(GetScreenWidth(), GetScreenHeight());
      } else {
        const int minutes = static_cast<int>(gameTime) / 60;
        const int seconds = static_cast<int>(gameTime) % 60;
        const int milliseconds = static_cast<int>(gameTime * 1000.0) % 1000;
        DrawText(TextFormat("Czas: %02d:%02d:%03d", minutes, seconds,
                            milliseconds),
                 20, 20, 24, RAYWHITE);
      }
    } else if (state == GameState::Paused) {
      camera.target = player.GetPosition();
      camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
      camera.rotation = 0.0f;
      camera.zoom = 1.0f;
      ClearBackground(BLACK);
      BeginMode2D(camera);
      mazeSystem.Draw();
      player.Draw();
      EndMode2D();
      pauseMenu.Draw();
    } else {
      ClearBackground(BLACK);
      mazeSystem.DrawMapOverlay(GetScreenWidth(), GetScreenHeight());
      gameOverScreen->Draw();
    }
    EndDrawing();
  }

  delete gameOverScreen;
  CloseWindow();
  return 0;
}
