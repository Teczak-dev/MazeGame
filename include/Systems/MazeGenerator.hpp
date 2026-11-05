#pragma once
#include "Systems/Tile.hpp"
#include <vector>
#include "raylib.h"

class MazeGenerator {
public:
  MazeGenerator(int width, int height);
  void generateMaze();
  Vector2 getStartPosition() const;
  const std::vector<std::vector<Tile>> &GetMaze() const;
  std::vector<std::vector<Tile>> &GetMaze();

private:
  int width;
  int height;
  std::vector<std::vector<Tile>> maze;
  Vector2 startPosition;
  void initializeMaze();
};
