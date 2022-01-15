//
// Created by Niels on 09/12/21.
//

#include "game/Game.h"

int Game::play()
{
    try
    {
        while (playing)
        {
            handleInput(_getch());
            if(ship.getState() == ShipStates::LEAVING)
            {
                auto s {std::make_unique<Sea>(ship.getDestination(), ship)};
                changeLocation(std::move(s));
                ship.setState(ShipStates::SAILING);
            }
        }
    }
    catch(const std::exception &e)
    {
        return EXIT_SUCCESS;
    }

    return 1;
}

Game::Game()
{
    DB::connect();

    //generate a random ship with a random amount of gold
    const auto randomShip{DB::selectData(
            "SELECT s.type, s.prijs, s.laadruimte, s.kanonnen, s.schadepunten, "
            "(SELECT bijzonderheid FROM bijzonderheden b WHERE b.id = sb.bijzonderheid_id) "
            "FROM schepen s INNER JOIN schepen_bijzonderheden sb on s.id = sb.schip_id ORDER BY random() LIMIT 1")[0]};
    const auto randomGold{RNG::generateRandomNumber(100000, 250000)};
//    ship = Ship{randomShip[0], std::stoi(randomShip[1]), std::stoi(randomShip[2]), std::stoi(randomShip[3]),
//                std::stoi(randomShip[4]), randomShip[5], randomGold};


    ship = Ship{randomShip[0], std::stoi(randomShip[1]), 500, 6,
                std::stoi(randomShip[4]), randomShip[5], randomGold};
    ship.boughtItem("cum", 350, 0);
    ship.boughtCanon(CanonType::LARGE, 2, 0);
    ship.boughtCanon(CanonType::MEDIUM, 2, 0);
    ship.boughtCanon(CanonType::SMALL, 2, 0);

    //generate a random harbor
    const auto randomHarbor{DB::selectData("SELECT id, haven FROM havens ORDER BY RANDOM() LIMIT 1")[0]};
    std::unique_ptr<Place> harbor(new Harbor(std::stoi(randomHarbor[0]), randomHarbor[1], ship));
    changeLocation(std::move(harbor));

    playing = true;

    //make cursor invisible for menu
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

void Game::handleInput(int key) const
{
    //double code for arrow key
    if (key == 224)
        return;

    currentLocation->handleInput(key);
}

template<typename T>
void Game::changeLocation(T location)
{
    currentLocation = move(location);
    menuHandler::resetCursor();
}
