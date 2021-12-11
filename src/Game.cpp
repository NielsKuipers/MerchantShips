//
// Created by Niels on 09/12/21.
//

#include "Game.h"
#include <sqlite/sqlite3.h>
#include <iostream>

int Game::play()
{
    sqlite3 *db;
    int conn{sqlite3_open("../database/handelsschepen.db", &db)};

    if(conn)
        std::cout << "can't open db" << sqlite3_errmsg(db);
    else
        std::cout << "epic";

    sqlite3_close(db);

    return 1;
}
