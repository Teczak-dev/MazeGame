#include "Systems/MazeSystem.hpp"
#include "Characters/Player.hpp"
#include "raylib.h"
#include <algorithm>
#include <cmath>

MazeSystem::MazeSystem(int width, int height, float tileSize)
    : generator(width, height), tileSize(tileSize) {}

void MazeSystem::SetSize(int width, int height) {
  generator = MazeGenerator(width, height);
}

void MazeSystem::Generate() {
  generator.generateMaze();
  playerPath.clear();
  revealedPathTiles = 0;
  pathRevealTimer = 0.0f;
  const Vector2 start = generator.getStartPosition();
  playerPath.emplace_back(static_cast<int>(start.x), static_cast<int>(start.y));
}

void MazeSystem::Update(float deltaTime, const Player &player) {
  (void)deltaTime;
  const Vector2 currentPosition = player.GetPosition();
  playerPosition = currentPosition;
  const auto &maze = generator.GetMaze();
  if (maze.empty() || maze.front().empty()) {
    return;
  }

  const int playerTileX =
      static_cast<int>(currentPosition.x / tileSize);
  const int playerTileY =
      static_cast<int>(currentPosition.y / tileSize);
  const int height = static_cast<int>(maze.size());
  const int width = static_cast<int>(maze.front().size());
  const int minX = std::max(0, playerTileX - viewRadius);
  const int maxX = std::min(width - 1, playerTileX + viewRadius);
  const int minY = std::max(0, playerTileY - viewRadius);
  const int maxY = std::min(height - 1, playerTileY + viewRadius);

  if (playerPath.empty() ||
      playerPath.back() != std::make_pair(playerTileX, playerTileY)) {
    playerPath.emplace_back(playerTileX, playerTileY);
  }

  auto &mutableMaze = generator.GetMaze();
  for (int y = minY; y <= maxY; ++y) {
    for (int x = minX; x <= maxX; ++x) {
      mutableMaze[y][x].isDiscovered = true;
    }
  }
}

void MazeSystem::StartPathReveal() {
  revealedPathTiles = 0;
  pathRevealTimer = 0.0f;
}

void MazeSystem::UpdatePathReveal(float deltaTime) {
  if (revealedPathTiles >= static_cast<int>(playerPath.size())) {
    return;
  }

  pathRevealTimer += deltaTime;
  constexpr float tileRevealTime = 0.06f;
  while (pathRevealTimer >= tileRevealTime &&
         revealedPathTiles < static_cast<int>(playerPath.size())) {
    pathRevealTimer -= tileRevealTime;
    ++revealedPathTiles;
  }
}

void MazeSystem::Draw() const {
  const auto &maze = generator.GetMaze();
  if (maze.empty() || maze.front().empty()) {
    return;
  }

  const int height = static_cast<int>(maze.size());
  const int width = static_cast<int>(maze.front().size());
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const Tile &tile = maze[y][x];
      if (!tile.isDiscovered) {
        continue;
      }

      Color color = {70, 70, 78, 255};
      if (tile.type == TILE_FLOOR) {
        color = {220, 220, 225, 255};
      } else if (tile.type == TILE_ENTRY) {
        color = GREEN;
      } else if (tile.type == TILE_EXIT) {
        color = YELLOW;
      }
      DrawRectangle(static_cast<int>(x * tileSize),
                    static_cast<int>(y * tileSize),
                    static_cast<int>(tileSize), static_cast<int>(tileSize),
                    color);
    }
  }

  const Color borderColor = {190, 60, 255, 255};
  DrawRectangleLinesEx({0.0f, 0.0f, width * tileSize, height * tileSize},
                       8.0f, borderColor);
}

void MazeSystem::DrawMapOverlay(int screenWidth, int screenHeight) const {
  const auto &maze = generator.GetMaze();
  if (maze.empty() || maze.front().empty()) {
    return;
  }

  const int height = static_cast<int>(maze.size());
  const int width = static_cast<int>(maze.front().size());
  const float margin = 80.0f;
  const float availableWidth = screenWidth - 2.0f * margin;
  const float availableHeight = screenHeight - 2.0f * margin;
  const float mapTileSize =
      std::min(availableWidth / width, availableHeight / height);
  const float mapWidth = width * mapTileSize;
  const float mapHeight = height * mapTileSize;
  const float mapX = (screenWidth - mapWidth) * 0.5f;
  const float mapY = (screenHeight - mapHeight) * 0.5f;

  DrawRectangle(0, 0, screenWidth, screenHeight, {8, 8, 14, 245});
  DrawText("MAPA - nacisnij M, aby wrocic", 30, 25, 24, RAYWHITE);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const Tile &tile = maze[y][x];
      if (!tile.isDiscovered) {
        continue;
      }

      Color color = {70, 70, 78, 255};
      if (tile.type == TILE_FLOOR) {
        color = {220, 220, 225, 255};
      } else if (tile.type == TILE_ENTRY) {
        color = GREEN;
      } else if (tile.type == TILE_EXIT) {
        color = YELLOW;
      }

      for (int pathIndex = 0; pathIndex < static_cast<int>(playerPath.size());
           ++pathIndex) {
        if (playerPath[pathIndex].first == x &&
            playerPath[pathIndex].second == y) {
          color = pathIndex < revealedPathTiles
                      ? Color{70, 210, 255, 255}
                      : Color{245, 245, 245, 255};
          break;
        }
      }

      DrawRectangle(static_cast<int>(mapX + x * mapTileSize),
                    static_cast<int>(mapY + y * mapTileSize),
                    static_cast<int>(std::ceil(mapTileSize)),
                    static_cast<int>(std::ceil(mapTileSize)), color);
    }
  }

  DrawRectangleLinesEx({mapX, mapY, mapWidth, mapHeight}, 6.0f,
                       {190, 60, 255, 255});

  const Vector2 minimapPlayerPosition = {
      mapX + playerPosition.x / tileSize * mapTileSize,
      mapY + playerPosition.y / tileSize * mapTileSize};
  DrawCircleV(minimapPlayerPosition, std::max(3.0f, mapTileSize * 0.35f),
              BLUE);
}

Vector2 MazeSystem::GetStartPosition() const {
  return generator.getStartPosition();
}

const std::vector<std::vector<Tile>> &MazeSystem::GetMaze() const {
  return generator.GetMaze();
}

bool MazeSystem::HasPlayerReachedExit(const Player &player) const {
  const Vector2 position = player.GetPosition();
  const int tileX = static_cast<int>(position.x / tileSize);
  const int tileY = static_cast<int>(position.y / tileSize);
  const auto &maze = generator.GetMaze();

  return tileY >= 0 && tileY < static_cast<int>(maze.size()) &&
         tileX >= 0 && tileX < static_cast<int>(maze[tileY].size()) &&
         maze[tileY][tileX].type == TILE_EXIT;
}
