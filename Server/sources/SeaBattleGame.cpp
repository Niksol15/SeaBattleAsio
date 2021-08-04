//
// Created by niksol on 17.02.2021.
//
#include "SeaBattleGame.h"
#include <iostream>

SeaBattleGame::BattlefieldType SeaBattleGame::generateBattlefield() {
    return {{{1, 1, 0, 1, 0, 0, 1, 1, 1, 1},
                    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
                    {0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
                    {0, 1, 0, 0, 1, 1, 1, 0, 0, 0},
                    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
                    {0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
                    {0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}
    };
}

SeaBattleGame::SeaBattleGame() : battlefield(generateBattlefield()),
                                 aliveCellCounter(0), shotCounter(0), hitCounter(0) {
    for (const auto &row: battlefield) {
        for (auto cell: row) {
            aliveCellCounter += cell;
        }
    }
}

std::string SeaBattleGame::Shot(int x, int y) {
    ++shotCounter;
    if (x >= 0 && x < BattlefieldSize && y >= 0 && y < BattlefieldSize) {
        if (battleField[x][y] == 1) {
            battleField[x][y] = -1;
            --aliveCellCounter;
            ++hitCounter;
            return aliveCellCounter == 0 ? "You win" : "You hit\n" + battlefieldToString();
        } else if (battleField[x][y] == 0) {
            battleField[x][y] = -2;
            return "You missed\n" + battlefieldToString();
        } else {
            return "You missed, this cell already hit\n" + battlefieldToString();
        }
    }
    return "You missed, out of range";
}

std::string Statistic() {
    return "Your statistic: " + std::to_string(hitCounter) + " hit, " + std::to_string(shotCounter) + " shot";
}

std::string NewGame() {
    battleField = DEFAULT_BATTLEFIELD;
    aliveCellCounter = 0;
    shotCounter = 0;
    hitCounter = 0;
    for (const auto &row: battleField) {
        for (const auto &cell: row) {
            if (cell == 1) {
                ++aliveCellCounter;
            }
        }
    }
    return "You are starting new game";
}

std::string battlefieldToString() {
    std::string res;
    res.reserve((battleField.size() + 2) * (battleField[0].size() + 2));
    res += " ---------- \n";
    for (const auto &row: battleField) {
        res += '|';
        for (const auto &cell: row) {
            if (cell == 1 || cell == 0) {
                res += ' ';
            } else if (cell == -1) {
                res += '*';
            } else {
                res += '0';
            }
        }
        res += "|\n";
    }
    res += " ---------- \n";
    return res;
}

private:
std::vector<std::vector<int>> battleField;
int aliveCellCounter;
int shotCounter;
int hitCounter;
};