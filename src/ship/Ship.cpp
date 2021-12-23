//
// Created by Niels on 11/12/21.
//

#include <iostream>
#include "ship/Ship.h"

Ship::Ship(const std::string &type, int price, int cargo, int canons, int health, const std::string &ability, int gold)
{
    this->type = type;
    this->price = price;
    this->cargoSpace = cargo;
    this->canonSpace = canons;
    this->health = health;
    this->ability = ability;
    this->currentGold = gold;
}

int Ship::getItemAmount(const std::string &item) const
{
    const auto &pos{goods.find(item)};
    if (pos == goods.end())
        return 0;
    else
        return pos->second;
}

void Ship::soldItem(const std::string &item, int amount, int earned)
{
    goods.at(item) -= amount;
    if (goods.at(item) == 0)
        goods.erase(item);

    cargoSpaceInUse -= amount;
    currentGold += earned;
}

void Ship::boughtItem(const std::string &item, int amount, int spent)
{
    goods[item] += amount;
    cargoSpaceInUse += amount;
    currentGold -= spent;
}

void Ship::boughtCanon(const std::string &canonType, int amount, int spent)
{
    canons[canonType] += amount;
    canonSpaceInUse += amount;
    currentGold -= spent;
}

void Ship::soldCanon(const std::string &canonType, int amount, int earned)
{
    canons.at(canonType) -= amount;
    if (canons.at(canonType) == 0)
        canons.erase(canonType);

    canonSpaceInUse -= amount;
    currentGold += earned;
}

void Ship::changeShip(std::tuple<std::string, int, int, int, int, std::string> &newShip)
{
    type = std::get<0>(newShip);
    price = std::get<1>(newShip);

    if (std::get<2>(newShip) < cargoSpaceInUse)
    {
        std::vector<std::string> toRemove{};

        for (auto &[key, val]: goods)
        {
            while (val > 0 && cargoSpaceInUse > std::get<2>(newShip))
            {
                val--;
                cargoSpaceInUse--;
            }

            if (val == 0)
                toRemove.emplace_back(key);
            else
            {
                for (const auto &rem: toRemove)
                    goods.erase(rem);
                break;
            }
        }
    }
    cargoSpace = std::get<2>(newShip);

    if (std::get<3>(newShip) < canonSpaceInUse)
    {
        std::vector<std::string> toRemove{};

        for (auto &[key, val]: canons)
        {
            while (val > 0 && canonSpaceInUse > std::get<3>(newShip))
            {
                val--;
                canonSpaceInUse--;
            }

            if (val == 0)
                toRemove.emplace_back(key);
            else
            {
                for (const auto &rem: toRemove)
                    canons.erase(rem);
                break;
            }
        }
    }
    canonSpace = std::get<3>(newShip);

    health = std::get<4>(newShip);
    ability = std::get<5>(newShip);
}

int Ship::displayShipInfo() const
{
    std::cout << "Your current ship is a " << type << " of type " << ability << " worth " << price << " gold."
              << std::endl << "Its current health is " << health << " and you have " << currentGold << " gold."
              << std::endl << "It currently has " << cargoSpaceInUse << " goods" << " and " << canonSpaceInUse
              << " canons."
              << std::endl
              << std::endl;

    return 4;
}

