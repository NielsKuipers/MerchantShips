//
// Created by Niels on 11/12/21.
//

#ifndef MERCHANTSHIPS_HARBOR_H
#define MERCHANTSHIPS_HARBOR_H

#include <string>
#include <interface/Place.h>
#include <game/menuHandler.h>
#include <game/DBManager.h>
#include <game/RandomGeneration.h>
#include <harbor/HarborStates.h>
#include <map>
#include <optional>
#include "ship/Ship.h"
#include <variant>

class Harbor : public Place
{
    void handleInput(int key) override;

    int id;
    std::string name;
    int minLine{0};
    std::vector<std::string> menu;
    std::vector<std::tuple<std::string, int, int>> goods{};
    HarborStates currentState{HarborStates::MENU};
    Ship &ship;

    void generateGoods();

    void displayShop();

    template<typename T>
    void setState(T state);

    void buyItem(int y);

    void sellItem(int y);

public:
    Harbor(int id, const std::string &name, Ship &ship);

    void displayMenu();
};


#endif //MERCHANTSHIPS_HARBOR_H
