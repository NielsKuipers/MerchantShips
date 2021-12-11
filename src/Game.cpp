//
// Created by Niels on 09/12/21.
//

#include "Game.h"
#include <sqlite/sqlite3.h>
#include <iostream>
#include <DBManager.h>

int Game::play()
{
    DB::connect();

    return 1;
}
