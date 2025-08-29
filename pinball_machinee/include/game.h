#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include <vector>

using namespace std;

struct GameState
{
  int A_score = 0;
  int B_score = 0;
  vector<String> gameLog;
  bool GameActive = true;
  unsigned long gameStartTime = 0;
};

typedef struct __attribute__((packed)) 
{
  uint8_t teamA_score;
  uint8_t teamB_score;
  uint8_t winner;
  uint8_t game_status;
} GameResult;

extern GameState game;

void processGoal(int holeIndex, String ballColor);
void endGame();
void sendGameResult(uint8_t winner);
void resetGame();

#endif
