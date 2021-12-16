//
// Created by Niels on 11/12/21.
//

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
    if(pos == goods.end())
        return 0;
    else
        return pos->second;
}

void Ship::soldItem(const std::string &item, int amount, int earned)
{
    goods.at(item) -= amount;
    cargoSpace += amount;
    currentGold += earned;
}

void Ship::boughtItem(const std::string &item, int amount, int spent)
{
    goods[item] += amount;
    cargoSpace -= amount;
    currentGold -= spent;
}
