//
// Created by Niels on 09/12/21.
//

#include "Game.h"

int Game::play()
{
//    DB::selectData("SELECT (SELECT haven FROM havens WHERE haven1_id=havens.id), h.haven, a.afstand "
//                   "FROM afstanden a INNER JOIN havens h on h.id = a.haven2_id "
//                   "WHERE haven1_id in (10) OR haven2_id in (10) AND haven1_id < haven2_id");

    while (playing)
    {

    }

    return 1;
}

Game::Game()
{
    DB::connect();
    const auto randomHarbor {DB::selectData("SELECT id, haven FROM havens ORDER BY RANDOM() LIMIT 1")[0]};
    currentLocation = Harbor(std::stoi(randomHarbor[0]), randomHarbor[1]);
}
