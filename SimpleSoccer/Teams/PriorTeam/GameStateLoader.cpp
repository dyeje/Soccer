#include "GameStateLoader.h"

GameStateLoader* GameStateLoader::Instance() {
  static GameStateLoader instance;
  return &instance;
}
