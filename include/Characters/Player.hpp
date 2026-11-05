#pragma once
#include "Systems/Tile.hpp"
#include "raylib.h"
#include <vector>

class Player {
public:
  Player(int startX, int startY);

  void Update(float deltaTime, const std::vector<std::vector<Tile>> &maze,
              float tileSize);
  void Draw() const;
  Vector2 GetPosition() const;
  bool CheckCircleTileCollision(Vector2 circlePos, float collisionRadius, int tileX,
                                int tileY, float tileSize);

private:
  Vector2 position;
  float speed;
  float radius = 12.0f;
};
