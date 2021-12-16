//
// Created by Niels on 09/12/21.
//

#include "game/Game.h"

int Game::play()
{
//    DB::selectData("SELECT (SELECT haven FROM havens WHERE haven1_id=havens.id), h.haven, a.afstand "
//                   "FROM afstanden a INNER JOIN havens h on h.id = a.haven2_id "
//                   "WHERE haven1_id in (10) OR haven2_id in (10) AND haven1_id < haven2_id");

    while (playing)
    {
        handleInput(_getch());
    }

    return 1;
}

Game::Game()
{
    DB::connect();
    const auto randomHarbor{DB::selectData("SELECT id, haven FROM havens ORDER BY RANDOM() LIMIT 1")[0]};
    changeLocation(Harbor{std::stoi(randomHarbor[0]), randomHarbor[1]});

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
    currentLocation = std::make_unique<T>(location);
    menuHandler::setCursor(currentLocation->getOptions()[0], 0, 0);
}
