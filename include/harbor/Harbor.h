//
// Created by Niels on 11/12/21.
//

#ifndef MERCHANTSHIPS_HARBOR_H
#define MERCHANTSHIPS_HARBOR_H

#include <string>
#include <interface/Place.h>
#include <game/UIHandler.h>
#include <game/DBManager.h>
#include <game/RandomGeneration.h>
#include <harbor/HarborStates.h>
#include <map>
#include <optional>
#include <../exceptions/QuitGame.h>
#include "ship/Ship.h"

class Harbor : public Place
{
    void handleInput(int key) override;

    int id;
    std::string name;
    std::vector<std::string> menu;
    std::vector<std::tuple<std::string, int, int>> goods{};
    std::vector<std::tuple<std::string, int, int>> canons{};
    std::vector<std::tuple<std::string, int, int, int, int, std::string>> ships{};
    std::vector<std::tuple<int, std::string, int>> locations{};
    HarborStates currentShop;
    HarborStates currentState;
    Ship &ship;

    void generateGoods();

    void displayShop();

    template<typename T>
    void setState(T state);

    void buyItem(int y);

    void sellItem(int y);

    void displayShips();

    void buyShip(int y);

    void getLocations();

    void displayDestinations();

    void leaveHarbor(int y);

    CanonType convertToCanon(const std::string &string);

public:
    Harbor(int id, const std::string &name, Ship &ship);

    void displayMenu();
};


#endif //MERCHANTSHIPS_HARBOR_H
