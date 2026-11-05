#include "Characters/Player.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <vector>

Player::Player(int startX, int startY)
    : position{startX * 32.0f + 16.0f, startY * 32.0f + 16.0f},
      speed(200.0f) {}

// Sprawdza kolizję koła o środku (circlePos) i promieniu (radius)
// z kafelkiem w siatce o współrzędnych (tileX, tileY) i rozmiarze (tileSize)
bool Player::CheckCircleTileCollision(Vector2 circlePos, float collisionRadius,
                                      int tileX, int tileY, float tileSize) {
  // 1. Wyznaczamy najbilszy punkt na kwadracie kafelka względem środka koła
  float tileMinX = tileX * tileSize;
  float tileMinY = tileY * tileSize;
  float tileMaxX = tileMinX + tileSize;
  float tileMaxY = tileMinY + tileSize;

  float nearestX = std::clamp(circlePos.x, tileMinX, tileMaxX);
  float nearestY = std::clamp(circlePos.y, tileMinY, tileMaxY);

  // 2. Obliczamy odległość od środka koła do tego punktu
  float deltaX = circlePos.x - nearestX;
  float deltaY = circlePos.y - nearestY;

  // Jeśli kwadrat odległości jest mniejszy niż promień do kwadratu -> mamy
  // kolizję!
  return (deltaX * deltaX + deltaY * deltaY) <
         (collisionRadius * collisionRadius);
}

Vector2 Player::GetPosition() const { return position; }

void Player::Update(float deltaTime, const std::vector<std::vector<Tile>> &maze,
                    float tileSize) {
  // 1. Zbieramy wektor kierunku z klawiatury
  Vector2 moveDir = {0.0f, 0.0f};
  if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    moveDir.y -= 1.0f;
  if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    moveDir.y += 1.0f;
  if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    moveDir.x -= 1.0f;
  if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    moveDir.x += 1.0f;

  // Normalizujemy wektor, żeby ruch po przekątnej nie był szybszy
  if (Vector2Length(moveDir) > 0.0f) {
    moveDir = Vector2Normalize(moveDir);
  }

  // --- OŚ X (Ruch i Kolizja) ---
  position.x += moveDir.x * speed * deltaTime;

  // Wyliczamy zakres kafelków wokół gracza, które musimy sprawdzić
  int minTileX = (int)((position.x - radius) / tileSize);
  int maxTileX = (int)((position.x + radius) / tileSize);
  int minTileY = (int)((position.y - radius) / tileSize);
  int maxTileY = (int)((position.y + radius) / tileSize);

  for (int y = minTileY; y <= maxTileY; ++y) {
    for (int x = minTileX; x <= maxTileX; ++x) {
      // Sprawdzamy czy nie wychodzimy poza tablicę i czy dany kafelek to ściana
      if (y >= 0 && y < (int)maze.size() && x >= 0 && x < (int)maze[0].size()) {
        if (maze[y][x].isSolid) {
          if (CheckCircleTileCollision(position, radius, x, y, tileSize)) {
            // Cofamy ruch w osi X
            if (moveDir.x > 0) { // Ruch w prawo
              position.x = x * tileSize - radius;
            } else if (moveDir.x < 0) { // Ruch w lewo
              position.x = (x + 1) * tileSize + radius;
            }
          }
        }
      }
    }
  }

  // --- OŚ Y (Ruch i Kolizja) ---
  position.y += moveDir.y * speed * deltaTime;

  // Aktualizujemy granice sprawdzania po zmianie pozycji Y
  minTileX = (int)((position.x - radius) / tileSize);
  maxTileX = (int)((position.x + radius) / tileSize);
  minTileY = (int)((position.y - radius) / tileSize);
  maxTileY = (int)((position.y + radius) / tileSize);

  for (int y = minTileY; y <= maxTileY; ++y) {
    for (int x = minTileX; x <= maxTileX; ++x) {
      if (y >= 0 && y < (int)maze.size() && x >= 0 && x < (int)maze[0].size()) {
        if (maze[y][x].isSolid) {
          if (CheckCircleTileCollision(position, radius, x, y, tileSize)) {
            // Cofamy ruch w osi Y
            if (moveDir.y > 0) { // Ruch w dół
              position.y = y * tileSize - radius;
            } else if (moveDir.y < 0) { // Ruch w górę
              position.y = (y + 1) * tileSize + radius;
            }
          }
        }
      }
    }
  }
}

void Player::Draw() const { DrawCircleV(position, radius, BLUE); }
