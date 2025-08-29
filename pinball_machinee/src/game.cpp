#include "game.h"
#include "sensor.h"
#include <WiFi.h>
#include <esp_now.h>

extern const uint8_t control_mac[6];

GameState game;

void processGoal(int holeIndex, String ballColor)
{
    String team = (holeIndex < 3) ? "A" : "B";
    String message = ballColor + " ball in Team " + team + " hole " + String((holeIndex % 3) + 1);

    if ((ballColor == "BLACK" && team == "A") || (ballColor == "GRAY" && team == "B"))
    {
        if (ballColor == "BLACK")
        {
            game.A_score += 5;
            game.gameLog.push_back(message + " -> Team A +5 points");
        }
        else
        {
            game.B_score += 5;
            game.gameLog.push_back(message + " -> Team B +5 points");
        }
    }
    else
    {
        if (ballColor == "BLACK")
        {
            game.A_score -= 2;
            game.gameLog.push_back(message + " -> Team A -2 points");
        }
        else
        {
            game.B_score -= 2;
            game.gameLog.push_back(message + " -> Team B -2 points");
        }
    }

    if (game.A_score < 0)
        game.A_score = 0;
    if (game.B_score < 0)
        game.B_score = 0;

    game.gameLog.push_back("Current Score - Team A: " + String(game.A_score) + ", Team B: " + String(game.B_score));

    if (game.A_score >= 50 || game.B_score >= 50)
    {
        endGame();
    }
}

void endGame()
{
    game.GameActive = false;

    String winner;
    uint8_t winnerCode;

    if (game.A_score > game.B_score)
    {
        winner = "Team A";
        winnerCode = 1;
    }
    else if (game.B_score > game.A_score)
    {
        winner = "Team B";
        winnerCode = 2;
    }
    else
    {
        winner = "Draw";
        winnerCode = 0;
    }

    Serial.println("=== GAME OVER ===");
    Serial.println("Winner: " + winner);
    Serial.println("Final Score - Team A: " + String(game.A_score) + ", Team B: " + String(game.B_score));

    sendGameResult(winnerCode);
}

void resetGame()
{
    game.A_score = 0;
    game.B_score = 0;
    game.GameActive = true;
    game.gameLog.clear();

    Serial.println("=== GAME RESET ===");
    Serial.println("Scores reset");
}

void sendGameResult(uint8_t winner)
{
    GameResult result;
    result.teamA_score = game.A_score;
    result.teamB_score = game.B_score;
    result.winner = winner;
    result.game_status = 1;

    esp_err_t res = esp_now_send(control_mac, (uint8_t *)&result, sizeof(result));
    if (res == ESP_OK)
    {
        Serial.println("Game result sent successfully");
    }
    else
    {
        Serial.println("Failed to send game result");
    }
}

void updateSendGameResult()
{
    static unsigned long lastScoreUpdate = 0;
    if (millis() - lastScoreUpdate > 5000)
    {
        lastScoreUpdate = millis();
        if (game.GameActive)
        {
            GameResult currentState;
            currentState.teamA_score = game.A_score;
            currentState.teamB_score = game.B_score;
            currentState.winner = 0;
            currentState.game_status = 0;

            esp_now_send(control_mac, (uint8_t *)&currentState, sizeof(currentState));
        }
    }
}
