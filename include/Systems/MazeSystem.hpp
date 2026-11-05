#pragma once

#include "Systems/MazeGenerator.hpp"
#include <utility>
#include <vector>

class Player;

class MazeSystem {
public:
  MazeSystem(int width, int height, float tileSize = 32.0f);

  void SetSize(int width, int height);
  void Generate();
  void Update(float deltaTime, const Player &player);
  void UpdatePathReveal(float deltaTime);
  void StartPathReveal();
  void Draw() const;
  void DrawMapOverlay(int screenWidth, int screenHeight) const;

  Vector2 GetStartPosition() const;
  const std::vector<std::vector<Tile>> &GetMaze() const;
  bool HasPlayerReachedExit(const Player &player) const;

private:
  MazeGenerator generator;
  float tileSize;
  int viewRadius = 4;
  Vector2 playerPosition = {0.0f, 0.0f};
  std::vector<std::pair<int, int>> playerPath;
  int revealedPathTiles = 0;
  float pathRevealTimer = 0.0f;
};
