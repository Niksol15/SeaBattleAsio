//
// Created by niksol on 17.02.2021.
//

#ifndef SERVER_SEABATTLEGAME_H
#define SERVER_SEABATTLEGAME_H

#include <string>
#include <array>
#include <cstdint>

class SeaBattleGame {
public:
    SeaBattleGame();

    std::string Shot(int x, int y);

    [[nodiscard]] std::string statistic() const;

    std::string NewGame();

    std::string battlefieldToString() const;

private:
    static constexpr std::size_t BattlefieldSize = 10;
    using BattlefieldType = std::array<std::array<int8_t , BattlefieldSize>, BattlefieldSize>;
    BattlefieldType battlefield;
    int aliveCellCounter;
    int shotCounter;
    int hitCounter;

    static BattlefieldType generateBattlefield();
};

#endif //SERVER_SEABATTLEGAME_H
