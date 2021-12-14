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

class Harbor : public Place
{
    void handleInput(int key) override;

    int id;
    std::string name;
    std::vector<std::string> menu;
    std::vector<std::tuple<std::string, int, int>> goods{};
    HarborStates currentState{HarborStates::MENU};

    void generateGoods();

public:
    Harbor(int id, const std::string &name);

    void setState(int number);

    void displayShop();
};


#endif //MERCHANTSHIPS_HARBOR_H
