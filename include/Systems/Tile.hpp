#pragma once

enum TileType { TILE_WALL = 0, TILE_FLOOR = 1, TILE_ENTRY = 2, TILE_EXIT = 3 };

struct Tile {
  TileType type = TILE_WALL;
  int textureID = 0;
  bool isSolid = true;
  bool isDiscovered = false;
};
