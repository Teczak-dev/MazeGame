#include "Systems/MazeGenerator.hpp"
#include <array>
#include <queue>
#include <random>

MazeGenerator::MazeGenerator(int width, int height)
    : width(width), height(height) {
  initializeMaze();
}

void MazeGenerator::initializeMaze() {
  maze.resize(height, std::vector<Tile>(width, Tile()));
}

void MazeGenerator::generateMaze() {
  // 1. Wypełniamy całą mapę ścianami (TILE_WALL)
  std::vector<std::vector<Tile>> grid(height, std::vector<Tile>(width, Tile()));

  // Stos do zapamiętywania ścieżki powrotnej (Backtracking)
  std::vector<Vector2> stack;

  std::random_device rd;
  std::mt19937 rng(rd());

  // Punkt startowy (1, 1) - zawsze nieparzyste współrzędne dla korytarza
  int startX = 1;
  int startY = 1;
  grid[startY][startX].type = TILE_FLOOR;
  stack.push_back({(float)startX, (float)startY});

  // Kierunki ruchu o 2 kafelki (przaskujemy przez ścianę)
  // {dx, dy}
  const Vector2 directions[4] = {
      {0, -2}, // Góra
      {0, 2},  // Dół
      {-2, 0}, // Lewo
      {2, 0}   // Prawo
  };

  while (!stack.empty()) {
    Vector2 current = stack.back();
    std::vector<int> validDirections;

    // Sprawdzamy, które z 4 kierunków są prawidłowe (wewnątrz mapy i
    // nieodwiedzone)
    for (int i = 0; i < 4; ++i) {
      int nx = (int)current.x + (int)directions[i].x;
      int ny = (int)current.y + (int)directions[i].y;

      if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1) {
        if (grid[ny][nx].type == TILE_WALL) {
          validDirections.push_back(i);
        }
      }
    }

    if (!validDirections.empty()) {
      // Wygrywa losowy kierunek
      std::uniform_int_distribution<int> dist(0,
                                              (int)validDirections.size() - 1);
      int chosenDir = validDirections[dist(rng)];

      int nx = (int)current.x + (int)directions[chosenDir].x;
      int ny = (int)current.y + (int)directions[chosenDir].y;

      // Wyburzamy ścianę POMIĘDZY obecnym kafelkiem a nowym
      int wallX = (int)current.x + (int)directions[chosenDir].x / 2;
      int wallY = (int)current.y + (int)directions[chosenDir].y / 2;

      grid[wallY][wallX].type = TILE_FLOOR;
      grid[wallY][wallX].isSolid = false;
      grid[ny][nx].type = TILE_FLOOR;
      grid[ny][nx].isSolid = false;

      stack.push_back({(float)nx, (float)ny});
    } else {
      // Ślepy zaułek - cofamy się
      stack.pop_back();
    }
  }

  // BFS znajduje kafelek najbardziej oddalony od wejścia po korytarzach.
  std::vector<std::vector<int>> distances(
      height, std::vector<int>(width, -1));
  std::queue<std::pair<int, int>> queue;
  const std::array<std::pair<int, int>, 4> neighbors = {
      std::pair<int, int>{0, -1}, std::pair<int, int>{0, 1},
      std::pair<int, int>{-1, 0}, std::pair<int, int>{1, 0}};

  distances[startY][startX] = 0;
  queue.push({startX, startY});
  int exitX = startX;
  int exitY = startY;

  while (!queue.empty()) {
    const auto [x, y] = queue.front();
    queue.pop();

    if (distances[y][x] > distances[exitY][exitX]) {
      exitX = x;
      exitY = y;
    }

    for (const auto [dx, dy] : neighbors) {
      const int nextX = x + dx;
      const int nextY = y + dy;
      if (nextX >= 0 && nextX < width && nextY >= 0 &&
          nextY < height && distances[nextY][nextX] == -1 &&
          grid[nextY][nextX].type != TILE_WALL) {
        distances[nextY][nextX] = distances[y][x] + 1;
        queue.push({nextX, nextY});
      }
    }
  }

  // Ustawiamy wejście i wyjście.
  grid[startY][startX].type = TILE_ENTRY;
  grid[startY][startX].isSolid = false;
  grid[startY][startX].isDiscovered = true;
  grid[exitY][exitX].type = TILE_EXIT;
  grid[exitY][exitX].isSolid = false;
  grid[exitY][exitX].isDiscovered = true;
  startPosition = {(float)startX, (float)startY};
  maze = grid;
}

Vector2 MazeGenerator::getStartPosition() const { return startPosition; }

const std::vector<std::vector<Tile>> &MazeGenerator::GetMaze() const {
  return maze;
}

std::vector<std::vector<Tile>> &MazeGenerator::GetMaze() { return maze; }
